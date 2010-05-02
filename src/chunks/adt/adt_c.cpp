#include "adt_c.h"

ModelMap_t Adt_c::model_map_;

Adt_c::Adt_c(Buffer_t *buffer, Indices32_t *obj_uids)
    : Chunk_c(buffer),
      obj_uids_(obj_uids),
      mhdr_(this, 0xc),
      mcin_(this, mhdr_.mcin_off),
      mmdx_(this, mhdr_.mmdx_off),
      mmid_(this, mhdr_.mmid_off),
      mwmo_(this, mhdr_.mwmo_off),
      mwid_(this, mhdr_.mwid_off),
      mddf_(this, mhdr_.mddf_off),
      modf_(this, mhdr_.modf_off) {
  // check if we have to get MH2O chunk
  if (mhdr_.mh2o_off) {
    off_t mh2o_off = mhdr_.mh2o_off;
    mh2o_ = Mh2oChunk_p(new Mh2oChunk_s(this, mh2o_off));
  }

  // retrieve mcnk chunks now we have the mcin info
  InitMcnks();
}

Adt_c::~Adt_c() {
  // destroy mcnk chunks
  for (int i = 0; i < 256; i++) {
    delete mcnks_[i];
  }
}

void Adt_c::InitMcnks() {
  mcnks_.reserve(256);
  for (int i = 0; i < 256; i++) {
    off_t mcnk_off = mcin_.mcnk_info[i].mcnk_off;
    mcnks_.push_back(new McnkChunk_s(this, mcnk_off));
  }
}

void Adt_c::CleanUp() {
  for (ModelMap_t::iterator model = model_map_.begin();
       model != model_map_.end();
       ++model) {
    delete model->second;
  }
  model_map_.clear();
}

void Adt_c::BuildTerrain(bool removeWet, Mesh_c *mesh) {
  Indices32_t idx;
  Vertices_t vtx;
  Normals_t norm;

  // reserve space for terrain
  vtx.reserve(256*145);
  norm.reserve(256*145);
  idx.reserve(256*768);

  // set position of mesh to first chunks position
  const Vec3_t &pos = mcnks_.front()->position;
  mesh->SetPositon(Vec3_t(-pos.y, 0, -pos.x));

  // cycle through all mcnks and retrieve their geometry: 16*16 chunks
  for (int y = 0; y < 16; y++) {
    for (int x = 0; x < 16; x++) {
      int i = y*16+x;

      Vertices_t mcnk_vtx, mcnk_norm;
      Indices32_t mcnk_idx;
      McnkChunk_s &mcnk = *(mcnks_[i]);

      mcnk.mcvt.GetIndices(&mcnk_idx);
      mcnk.mcvt.GetVertices(&mcnk_vtx);
      mcnk.mcnr.GetNormals(&mcnk_norm);

      // mark holes
      for (int hy = 0; hy < 8; hy++) {
        for (int hx = 0; hx < 8; hx++) {
          // if hole is found mark all indices (12 of them) with uint max
          if(mcnk.holes & (1 << (hy/2 * 4 + hx/2))) {
            int cur_hi = hy*8*12 + hx*12;
            for (int hi = 0; hi < 12; hi++) {
              mcnk_idx[cur_hi+hi] = -1;
            }
          }
        }
      }

      // translate mcnk vertices to correctly position them next to each other
      Vec3_t mcnk_pos(TU*16*x, mcnk.position.z, TU*16*y);
      TranslateVertices(mcnk_pos, &mcnk_vtx, 0, mcnk_vtx.size());

      // merge all geometry information
      InsertIndices(mcnk_idx, vtx.size(), &idx);
      vtx.insert(vtx.end(), mcnk_vtx.begin(), mcnk_vtx.end());
      norm.insert(norm.end(), mcnk_norm.begin(), mcnk_norm.end());
    }
  }

  // Find all water cells in a terrain. Water is usually above terrain ;)
  // so we just have to remove every terrain cell covered by water.

  // The concept behind all these nested loops is quite easy:
  // The first two loops will cycle through our 16*16 = 256 map chunks ..
  int wet_num = 0;
  if (removeWet && (mh2o_.get() != NULL)) {
    for (int y = 0; y < 16; y++) {
      for (int x = 0; x < 16; x++) {
        int chunk_idx = y*16+x; // this our chunk index
        // .. then we check if we have any water in this chunk at all ..
        for (uint32_t l = 0; l < mh2o_->content[chunk_idx].num_layers; l++) {
          // .. so we have water lets just get the water mask.
          const uint64_t &mask = mh2o_->content[chunk_idx].masks[l];
          // The mask is a 64bit value which has a bit set for every water cell.
          for (int wy = 0; wy < 8; wy++) {
            for (int wx = 0; wx < 8; wx++) {
              int cell_idx = wy*8+wx; // water cell index
              // Do we have water in this cell?
              if (mask & (1ULL << cell_idx)) {
                int terr_idx = y*16*64+x*64 + cell_idx; // terrain index
                // A terrain cell is made up of 12 indices which describe
                // 4 polygons that make up a cell in our map chunk.
                for (int i = 0; i < 12; i++) {
                  idx[terr_idx*12+i] = -1;  // mark every index with uint max
                  wet_num++;
                }
              }
            }
          }
        }
      }
    }
  }

  RearrangeBuffers(&idx, &vtx, &norm);
  mesh->SetColors(&Colors_t(vtx.size(), 0xff127e14)); // ABGR
  mesh->SetGeometry(&idx, &vtx, &norm);
}

void Adt_c::LoadDoodads(MpqHandler_c &mpq_h, bool loadSkin) {
  for (McnkChunks_t::iterator mcnk = mcnks_.begin();
       mcnk != mcnks_.end();
       ++mcnk) {
    for (Indices32_t::iterator off = (*mcnk)->mcrf.doodad_offs.begin();
        off != (*mcnk)->mcrf.doodad_offs.end();
         ++off) {
      const MddfChunk_s::DoodadInfo_s &info = mddf_.doodad_infos.at(*off);
      // check if obj with unique id has already been placed
      if (UidAlreadyIn(info.uid)) { continue; }

      // replace false extensions with right one
      std::string filename(mmdx_.m2_names.c_str()+mmid_.name_offs.at(info.id));
      RreplaceWoWExt(ToLower(filename), ".mdx", ".m2", &filename);

      doodads_.push_back(AdtDoodad_s());
      AdtDoodad_s &doodad = doodads_.back();

      // use collision (bounding volume) models or not
      doodad.m2 = GetM2(mpq_h, filename, loadSkin);
      doodad.info = &info;
    }
  }
}

void Adt_c::GetDoodads(Meshes_t *meshes) {
   for (AdtDoodads_t::iterator doodad = doodads_.begin();
       doodad != doodads_.end();
       ++doodad) {
    const MddfChunk_s::DoodadInfo_s &info = *doodad->info;
    const M2_c *m2 = doodad->m2;

    // add new doodad
    meshes->push_back(Mesh_c());
    Mesh_c *mesh = &meshes->back();

    // if we have a skin we get the real mesh
    if(m2->skin.get() != NULL) {
      m2->GetMesh(*m2->skin, mesh);
    } else {
      m2->GetBVMesh(mesh);
    }

    // don't forget to readjust positions and rotations!
    mesh->SetPositon(Vec3_t(info.pos.x-17066.666666f, info.pos.y, info.pos.z-17066.666666f));
    mesh->SetRotation(Vec3_t(-info.rot.x, info.rot.y-90, info.rot.z-90));
    mesh->SetScale(info.scale/1024.0f);
  }
}

M2_c* Adt_c::GetM2(MpqHandler_c &mpq_h, const std::string &filename, bool loadSkin) {
  // check if m2 is already in our map
  ModelMap_t::iterator found = model_map_.find(filename);
  if (found != model_map_.end()) {
    return reinterpret_cast<M2_c*>(found->second);
  } else {
    // not in map, so load it
    Buffer_t buf;
    mpq_h.LoadFile(filename.c_str(), &buf);
    // create new entry in map
    M2_c *m2 = new M2_c(&buf);
    model_map_.insert(ModelPair_t(filename, m2));

    if (loadSkin) {
      std::string skin_name(filename);
      RreplaceWoWExt(ToLower(skin_name), ".m2", "00.skin", &skin_name);
      // load skin
      mpq_h.LoadFile(skin_name.c_str(), &buf);
      m2->skin = Skin_p(new Skin_c(&buf));
    }

    return m2;
  }
}

void Adt_c::LoadWmos(MpqHandler_c &mpq_h, bool loadSkin) {
  for (ModfChunk_s::WmoInfos_t::iterator wmo_info = modf_.wmo_infos.begin();
       wmo_info != modf_.wmo_infos.end();
       ++wmo_info) {
    // check if obj with unique id has already been placed
    if (UidAlreadyIn(wmo_info->uid)) { continue; }

    // replace false extensions with right one
    std::string filename;
    filename = mwmo_.wmo_names.c_str()+mwid_.name_offs.at(wmo_info->id);

    wmos_.push_back(Wmo_s());
    Wmo_s &wmo = wmos_.back();

    wmo.wmo = GetWmo(mpq_h, filename, loadSkin);
    wmo.info = &(*wmo_info);
  }
}

Wmo_c* Adt_c::GetWmo(MpqHandler_c &mpq_h, const std::string &filename, bool loadSkin) {
  // check if wmo is already in our map
  ModelMap_t::iterator found = model_map_.find(filename);
  if (found != model_map_.end()) {
    return reinterpret_cast<Wmo_c*>(found->second);
  } else {
    // not in map, so load it
    Buffer_t buf;
    mpq_h.LoadFile(filename.c_str(), &buf);
    // create new entry in map
    Wmo_c *wmo = new Wmo_c(&buf, filename, &model_map_);
    model_map_.insert(ModelPair_t(filename, wmo));
    wmo->LoadWmo(mpq_h);
    wmo->LoadDoodads(mpq_h, loadSkin);

    return wmo;
  }
}

void Adt_c::GetWmos(Meshes_t *wmos, Meshes_t *doodads) const {
  for (Wmos_t::const_iterator wmo = wmos_.begin();
       wmo != wmos_.end();
       ++wmo) {
    const ModfChunk_s::WmoInfo_s &info = *wmo->info;

    // add new wmo
    wmos->push_back(Mesh_c());
    Mesh_c *wmo_mesh = &wmos->back();
    // don't forget to readjust positions and rotations!
    wmo_mesh->SetPositon(Vec3_t(info.pos.x-17066.666666f, info.pos.y, info.pos.z-17066.666666f));
    wmo_mesh->SetRotation(Vec3_t(-info.rot.x, info.rot.y-90, info.rot.z-90));

    wmo->wmo->GetWmo(wmo_mesh);
    wmo->wmo->GetDoodads(doodads, wmo_mesh);
  }
}

bool Adt_c::UidAlreadyIn(uint32_t uid) const {
  Indices32_t::iterator found;
  found = std::find(obj_uids_->begin(), obj_uids_->end(), uid);
  // find unique identifier ..
  if (found != obj_uids_->end()) {
    return true;
  } else {
    // .. push new uid cause we didn't see it until now
    obj_uids_->push_back(uid);
    return false;
  }
}

/*void Adt_c::BuildWater() {
  Mh2oChunk_s &mh2o = mhdr_.mh2o;
  if (mh2o.heights.size() <= 0) return;

  for (int y = 0; y < 16; y++) {
    for (int x = 0; x < 16; x++) {
      int chunk_idx = y*16+x;

      if (mh2o.heights[chunk_idx].val.size()) {
        Mh2oChunk_s::Mh2oHeights_s &heightmap = mh2o.heights[chunk_idx];
        glm::vec3 &pos = mhdr_.mcin.mcnks[chunk_idx].position;

        int hw = heightmap.w+1; // heights width (max. 9)
        int hh = heightmap.h+1; // heights height (max. 9)

        Points_t vertices(hw*hh);
        Points_t normals(hw*hh, glm::vec3(0, 1, 0));
        Indices32_t indices(heightmap.w*heightmap.h*6);

        //std::cout << std::hex << heightmap.mask << std::endl;
        for (uint32_t water_y = 0; water_y < heightmap.h; water_y++) {
          for (uint32_t water_x = 0; water_x < heightmap.w; water_x++) {
            // positions inside the chunk (8x8, offsets included)
            int cur_x = water_x+heightmap.x;
            int cur_y = water_y+heightmap.y;

            // the mask indicates where there's water to render
            if (heightmap.mask & (1ULL << (cur_y*8+cur_x))) {
              // indices for water heights
              int htl = hw*water_y+water_x;       // top left
              int htr = hw*water_y+water_x+1;     // top right
              int hbl = hw*(water_y+1)+water_x;   // bottom left
              int hbr = hw*(water_y+1)+water_x+1; // bottom right

              // width and depth values for our water planes
              float left = cur_x*TU*2-TU - pos.y;
              float right = cur_x*TU*2 + TU*2 - pos.y ;
              float top = cur_y*TU*2 - pos.x;
              float bottom = cur_y*TU*2 + TU*2 - pos.x;

              // actual vertices
              vertices[htl] = glm::vec3(left, heightmap.val.at(htl), top);
              vertices[htr] = glm::vec3(right, heightmap.val.at(htr), top);
              vertices[hbl] = glm::vec3(left, heightmap.val.at(hbl), bottom);
              vertices[hbr] = glm::vec3(right, heightmap.val.at(hbr), bottom);

              // 2*3 vertices -> 1 water quad
              uint32_t idx_pad = water_y*6*heightmap.w+water_x*6;
              indices.at(idx_pad+0) = htl;
              indices.at(idx_pad+1) = htr;
              indices.at(idx_pad+2) = hbr;
              indices.at(idx_pad+3) = hbr;
              indices.at(idx_pad+4) = hbl;
              indices.at(idx_pad+5) = htl;
            }
          }
        }

        InsertIndices(indices, water_.vtx.size(), &water_.idx);
        water_.vtx.insert(water_.vtx.end(), vertices.begin(), vertices.end());
        water_.norm.insert(water_.norm.end(), normals.begin(), normals.end());
        water_.col.insert(water_.col.end(), vertices.size(), 0xffff0000);
      }
    }
  }
}*/

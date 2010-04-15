#include "adt_c.h"

Indices32_t Adt_c::obj_uids_;

Adt_c::Adt_c(Buffer_t *buffer, MpqHandler_c &mpq_h)
    : Chunk_c(buffer), mhdr_(this) {
  GetSubChunk(0x0c, &mhdr_); // GetSubChunk("RDHM", 4, &mhdr_);

  BuildTerrain(true);
  //BuildWater();
  GetDoodads(mpq_h, true); // true = use bounding volume, false = real mesh
  GetWmos(mpq_h);
}

Adt_c::~Adt_c() {
  for (M2Map_t::iterator m2 = m2_map_.begin(); m2 != m2_map_.end(); ++m2) {
    delete m2->second;
  }
}

void Adt_c::BuildTerrain(bool removeWet) {
  Points_t &vtx = terrain_.vtx;
  Points_t &norm = terrain_.norm;
  Indices32_t &idx = terrain_.idx;
  // reserve space for terrain
  vtx.reserve(256*145);
  norm.reserve(256*145);
  idx.reserve(256*768);
  terrain_.col.insert(terrain_.col.end(), 256*768, 0xff127e14); // ABGR

  // cycle through all mcnks and retrieve their geometry: 16*16 chunks
  for (McinChunk_s::McnkChunks_t::iterator mcnk = mhdr_.mcin.mcnks.begin();
       mcnk != mhdr_.mcin.mcnks.end();
       ++mcnk) {
    Points_t vertices, normals;
    Indices32_t indices;
    mcnk->mcvt.GetVertices(&vertices);
    mcnk->mcnr.GetNormals(&normals);
    mcnk->mcvt.GetIndices(&indices);

    // merge all geometry information
    InsertIndices(indices, vtx.size(), &idx);
    vtx.insert(vtx.end(), vertices.begin(), vertices.end());
    norm.insert(norm.end(), normals.begin(), normals.end());
  }

  // STEP 1: Find all water cells in a terrain. Water is usually above terrain ;)
  //         so we just have to remove every terrain cell covered by water.

  // The concept behind all these nested loops is quite easy:
  // The first two loops will cycle through our 16*16 = 256 map chunks ..
  if (removeWet) {
    for (int y = 0; y < 16; y++) {
      for (int x = 0; x < 16; x++) {
        int chunk_idx = y*16+x; // this our chunk index
        // .. then we check if we have any water in this chunk at all ..
        if (mhdr_.mh2o.heights[chunk_idx].val.size()) {
          // .. so we have water lets just get the water mask.
          const uint64_t &mask = mhdr_.mh2o.heights[chunk_idx].mask;
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
                  idx[terr_idx*12+i] = -1;  // mark every index with max uint
                }
              }
            }
          }
        }
      }
    }
  }

  // STEP 2: Remove all marked indices.
  for (Indices32_t::iterator rem_terr = idx.begin();
       rem_terr != idx.end();
       ++rem_terr) {
    if (*rem_terr == -1) {
      rem_terr = idx.erase(rem_terr); // returns element after deleted one ..
      rem_terr--;                     // .. step back and continue
    }
  }
}

void Adt_c::BuildWater() {
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
}

void Adt_c::GetDoodads(MpqHandler_c &mpq_h, bool useCollisionModel) {
  MddfChunk_s &mddf = mhdr_.mddf;
  MmdxChunk_s &mmdx = mhdr_.mmdx;
  MmidChunk_s &mmid = mhdr_.mmid;

  for (McinChunk_s::McnkChunks_t::iterator mcnk = mhdr_.mcin.mcnks.begin();
       mcnk != mhdr_.mcin.mcnks.end();
       ++mcnk) {
    for (Indices32_t::iterator offset = mcnk->mcrf.doodad_offsets.begin();
         offset != mcnk->mcrf.doodad_offsets.end();
         ++offset) {
      MddfChunk_s::DoodadInfo_s &info = mddf.doodad_info.at(*offset);
      // check if obj with unique id has already been placed
      if (CheckUid(info.uid)) { continue; }

      std::string filename(mmdx.m2_names.c_str()+mmid.name_offsets.at(info.id));

      // replace false extensions with right one
      std::string m2_name = filename;
      size_t ext_pos = m2_name.rfind(".mdx");
      if (ext_pos != std::string::npos) { m2_name.replace(ext_pos, 4, ".m2"); }

      m2s_.push_back(Mesh_s());
      Mesh_s &m2_mesh = m2s_.back();

      // use collision (bounding volume) models or not
      if (useCollisionModel) {
        GetM2(mpq_h, filename, false)->GetBVMesh(&m2_mesh);
      } else {
        M2_c *m2 = GetM2(mpq_h, filename, false);
        m2->GetMesh(*m2->skin, &m2_mesh);
      }
    }
  }
}

M2_c* Adt_c::GetM2(MpqHandler_c &mpq_h, const std::string &filename, bool loadSkin) {
  // check if m2 with filename is in our map
  M2Map_t::iterator found = m2_map_.find(filename);
  if (found != m2_map_.end()) {
    return found->second;
  } else {
    Buffer_t m2_buf;
    // not in map, so load it
    mpq_h.LoadFile(filename.c_str(), &m2_buf);

    // create new entry in map
    M2_c *m2 = new M2_c(&m2_buf);
    m2_map_.insert(M2Pair_t(filename, m2));

    if (loadSkin) {
      std::string skin_fn = filename;
      skin_fn.replace(skin_fn.size()-4, 3, "00.skin");

      Buffer_t skin_buf;
      mpq_h.LoadFile(skin_fn.c_str(), &skin_buf);
      Skin_c *skin = new Skin_c(&skin_buf);
      m2->skin = skin;
    }

    return m2;
  }
}

void Adt_c::GetWmos(MpqHandler_c &mpq_h) {
  ModfChunk_s &modf = mhdr_.modf;
  MwidChunk_s &mwid = mhdr_.mwid;
  MwmoChunk_s &mwmo = mhdr_.mwmo;

  for (ModfChunk_s::WmoInfo_t::iterator wmo_info = modf.wmo_info.begin();
       wmo_info != modf.wmo_info.end();
       ++wmo_info) {
    // check if object with unique id has already been placed in our world
    Indices32_t::iterator found;
    found = std::find(obj_uids_.begin(), obj_uids_.end(), wmo_info->uid);
    if (found != obj_uids_.end()) { continue; }
    obj_uids_.push_back(wmo_info->uid);

    // get wmo filename
    const char *fn = mwmo.wmo_names.c_str()+mwid.name_offsets.at(wmo_info->id);
    Buffer_t wmo_buf;
    mpq_h.LoadFile(fn, &wmo_buf);
    Wmo_c wmo(&wmo_buf, fn, mpq_h);
    // create new wmo
    wmos_.push_back(Mesh_s());
    Mesh_s &wmo_mesh = wmos_.back();

    Points_t wmo_vtx = wmo.vertices();
    TransWoWToRH(wmo_info->position, wmo_info->orientation, 1.0f, &wmo_vtx);

    InsertIndices(wmo.indices(), wmo_mesh.vtx.size(), &wmo_mesh.idx);
    wmo_mesh.vtx.insert(wmo_mesh.vtx.end(), wmo_vtx.begin(), wmo_vtx.end());
    wmo_mesh.norm.insert(wmo_mesh.norm.end(), wmo.normals().begin(), wmo.normals().end());
    wmo_mesh.col.insert(wmo_mesh.col.end(), wmo.colors().begin(), wmo.colors().end());
  }
}

bool Adt_c::CheckUid(uint32_t uid) const {
  Indices32_t::iterator found;
  found = std::find(obj_uids_.begin(), obj_uids_.end(), uid);
  if (found != obj_uids_.end()) {
    return true;
  } else {
    obj_uids_.push_back(uid);
    return false;
  }
}

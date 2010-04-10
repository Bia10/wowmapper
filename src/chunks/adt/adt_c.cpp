#include "adt_c.h"

Indices32_t Adt_c::obj_uids_;

Adt_c::Adt_c(Buffer_t *buffer, MpqHandler_c &mpq_h)
    : Chunk_c(buffer), mhdr_(this) {
  GetSubChunk(0x0c, &mhdr_); //GetSubChunk("RDHM", 4, &mhdr_);

  GenerateTerrain();
  GenerateWater();
  InsertDoodads(mpq_h, true);

  ModfChunk_s &modf = mhdr_.modf;
  MwidChunk_s &mwid = mhdr_.mwid;
  MwmoChunk_s &mwmo = mhdr_.mwmo;

  for (ModfChunk_s::WmoInfo_t::iterator wmo_info = modf.wmo_info.begin();
       wmo_info != modf.wmo_info.end();
       ++wmo_info) {
    // check if obj with unique id has already been placed
    Indices32_t::iterator found;
    found = std::find(obj_uids_.begin(), obj_uids_.end(), wmo_info->uid);
    if (found != obj_uids_.end()) { continue; }
    obj_uids_.push_back(wmo_info->uid);

    // get wmo filename
    std::string filename(mwmo.wmo_names.c_str()+mwid.name_offsets.at(wmo_info->id));

    Buffer_t wmo_buf;
    mpq_h.LoadFile(filename.c_str(), &wmo_buf);
    std::cout << "Load WMO: " << filename << " " << wmo_buf.size()/1024 << " kb" << std::endl;
    Wmo_c wmo(&wmo_buf, filename, mpq_h);

    InsertIndices(wmo.indices(), vertices_.size(), &indices_);

    Points_t vertices = wmo.vertices();
    TransformVertices(wmo_info->position, wmo_info->orientation, 1.0f, &vertices);
    vertices_.insert(vertices_.end(), vertices.begin(), vertices.end());

    normals_.insert(normals_.end(), wmo.normals().begin(), wmo.normals().end());
    colors_.insert(colors_.end(), wmo.colors().begin(), wmo.colors().end());
  }
}

Adt_c::~Adt_c() {
  for (M2Map_t::iterator dd = m2_map_.begin(); dd != m2_map_.end(); ++dd) {
    delete dd->second;
  }
}

void Adt_c::GenerateTerrain() {
  vertices_.reserve(256*145);
  normals_.reserve(256*145);
  indices_.reserve(256*768);
  colors_.insert(colors_.end(), 256*145, 0xff127e14); // ABGR

  for (McinChunk_s::McnkChunks_t::iterator mcnk = mhdr_.mcin.mcnks.begin();
       mcnk != mhdr_.mcin.mcnks.end();
       ++mcnk) {
    Points_t vertices, normals;
    Indices32_t indices;
    mcnk->mcvt.GetVertices(&vertices);
    mcnk->mcnr.GetNormals(&normals);
    mcnk->mcvt.GetIndices(&indices);

    InsertIndices(indices, vertices_.size(), &indices_);

    // insert all vertices, normals, indices into their bigger arrays
    vertices_.insert(vertices_.end(), vertices.begin(), vertices.end());
    normals_.insert(normals_.end(), normals.begin(), normals.end());
  }
}

void Adt_c::GenerateWater() {
  Mh2oChunk_s &mh2o = mhdr_.mh2o;
  if (mh2o.heights.size() <= 0) return;

  for (int tile_y = 0; tile_y < 16; tile_y++) {
    for (int tile_x = 0; tile_x < 16; tile_x++) {
      int cur_tile = tile_y*16+tile_x;
      if (mh2o.heights[cur_tile].val.size()) {
        Mh2oChunk_s::Mh2oHeights_s &heightmap = mh2o.heights[cur_tile];
        glm::vec3 &pos = mhdr_.mcin.mcnks[cur_tile].position;

        int hw = heightmap.w+1; // heights width (max. 9)
        int hh = heightmap.h+1; // heights height (max. 9)

        Points_t vertices(hw*hh);
        Points_t normals(hw*hh, glm::vec3(0, 1, 0));
        Indices32_t indices(heightmap.w*heightmap.h*6);

        for (uint32_t water_y = 0; water_y < heightmap.h; water_y++) {
          for (uint32_t water_x = 0; water_x < heightmap.w; water_x++) {
            // positions inside the tile (8x8, offsets included)
            int cur_x = water_x+heightmap.x;
            int cur_y = water_y+heightmap.y;

            if (heightmap.mask & (1ULL << (cur_y*8+cur_x))) {
              int htl = hw*water_y+water_x;       // top left
              int htr = hw*water_y+water_x+1;     // top right
              int hbl = hw*(water_y+1)+water_x;   // bottom left
              int hbr = hw*(water_y+1)+water_x+1; // bottom right

              float left = (cur_x*UNIT*2 - UNIT) - pos.y + 17066.66667f;
              float right = (cur_x*UNIT*2 + UNIT) - pos.y + 17066.66667f;
              float top = (cur_y*UNIT*2 - UNIT) - pos.x + 17066.66667f;
              float bottom = (cur_y*UNIT*2 + UNIT) - pos.x + 17066.66667f;

              vertices[htl] = glm::vec3(left, heightmap.val.at(htl), top);
              vertices[htr] = glm::vec3(right, heightmap.val.at(htr), top);
              vertices[hbl] = glm::vec3(left, heightmap.val.at(hbl), bottom);
              vertices[hbr] = glm::vec3(right, heightmap.val.at(hbr), bottom);

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

        InsertIndices(indices, vertices_.size(), &indices_);
        vertices_.insert(vertices_.end(), vertices.begin(), vertices.end());
        normals_.insert(normals_.end(), normals.begin(), normals.end());
        colors_.insert(colors_.end(), vertices.size(), 0xffff0000);
      }
    }
  }
}

void Adt_c::InsertDoodads(MpqHandler_c &mpq_h, bool useCollisionModel) {
  MddfChunk_s &mddf = mhdr_.mddf;
  MmdxChunk_s &mmdx = mhdr_.mmdx;
  MmidChunk_s &mmid = mhdr_.mmid;

  for (McinChunk_s::McnkChunks_t::iterator mcnk = mhdr_.mcin.mcnks.begin();
       mcnk != mhdr_.mcin.mcnks.end();
       ++mcnk) {
    // get doodad filenames
    Filenames_t filenames;
    for (Indices32_t::iterator offset = mcnk->mcrf.doodad_offsets.begin();
         offset != mcnk->mcrf.doodad_offsets.end();
         ++offset) {
      MddfChunk_s::DoodadInfo_s &info = mddf.doodad_info.at(*offset);
      // check if obj with unique id has already been placed
      Indices32_t::iterator found;
      found = std::find(obj_uids_.begin(), obj_uids_.end(), info.uid);
      if (found != obj_uids_.end()) { continue; }
      obj_uids_.push_back(info.uid);

      std::string filename(mmdx.m2_names.c_str()+mmid.name_offsets.at(info.id));

      // replace false extensions with right one
      std::string m2_name = filename;
      size_t ext_pos = m2_name.rfind(".mdx");
      if (ext_pos != std::string::npos) { m2_name.replace(ext_pos, 4, ".m2"); }

      // use collision (bounding volume) models or not
      if (useCollisionModel) {
        // look for already loaded m2 and use them instead of reloading them
        M2Map_t::iterator found = m2_map_.find(m2_name);
        if (found != m2_map_.end()) {
          InsertBoundingTriangles(*(*found).second, info);
        } else {
          // load m2 model file
          Buffer_t m2_buf;
          mpq_h.LoadFile(m2_name.c_str(), &m2_buf);

          // create new entry in map
          M2_c *m2 = new M2_c(&m2_buf);
          m2_map_.insert(M2Pair_t(m2_name, m2));

          // insert triangles
          InsertBoundingTriangles(*m2, info);
        }
      } else {
        // look for already loaded m2 and use them instead of reloading them
        M2SkinMap_t::iterator found = m2skin_map_.find(m2_name);
        if (found != m2skin_map_.end()) {
          M2Skin_s &m2skin = found->second;
          InsertVisibleTriangles(*m2skin.m2, *m2skin.skin, info);
        } else {
          // load m2 because we don't have it
          Buffer_t m2skin_buf;
          mpq_h.LoadFile(m2_name.c_str(), &m2skin_buf);
          M2_c *m2 = new M2_c(&m2skin_buf);

          // construct skin name
          std::string skin_name = m2_name;
          ext_pos = skin_name.rfind(".m2");
          if (ext_pos != std::string::npos) {
            skin_name.replace(ext_pos, 3, "00.skin");
          }

          // load skin and ultimately insert it into the map with the m2
          mpq_h.LoadFile(skin_name.c_str(), &m2skin_buf);
          Skin_c *skin = new Skin_c(&m2skin_buf);

          M2Skin_s m2skin = {m2, skin};
          m2skin_map_.insert(M2SkinPair_t(m2_name, m2skin));

          InsertVisibleTriangles(*m2, *skin, info);
        }
      }
    }
  }
}

void Adt_c::InsertVisibleTriangles(const M2_c &m2, const Skin_c &skin,
                                  const MddfChunk_s::DoodadInfo_s &info) {
  Indices32_t indices;
  skin.GetIndices(&indices);

  InsertIndices(indices, vertices_.size(), &indices_);

  Points_t vertices, normals;
  m2.GetVertices(&vertices);
  m2.GetNormals(&normals);

  colors_.insert(colors_.end(), vertices.size(), 0xff11e2ea);

  TransformVertices(info.position, info.orientation,
      info.scale/1024.0f, &vertices);

  normals_.insert(normals_.end(), normals.begin(), normals.end());
  vertices_.insert(vertices_.end(), vertices.begin(), vertices.end());
}

void Adt_c::InsertBoundingTriangles(const M2_c &m2,
                                    const MddfChunk_s::DoodadInfo_s &info) {
  if (m2.bv_indices().size() <= 0) { return; }
  // insert indices
  Indices32_t indices(m2.bv_indices().begin(), m2.bv_indices().end());
  InsertIndices(indices, vertices_.size(), &indices_);
  colors_.insert(colors_.end(), m2.bv_vertices().size(), 0xff11e2ea); // ABGR

  // create vertices and normals, resize and insert normals
  Points_t vertices = m2.bv_vertices();
  Points_t normals(vertices.size(), glm::vec3(0.0f));

  TransformVertices(info.position, info.orientation,
      info.scale/1024.0f, &vertices);

  // wow uses triangle normals for bounding volumes ...
  for (uint32_t i = 0; i < indices.size() / 3; i++) {
     normals.at(indices[i*3+0]) += m2.bv_normals().at(i);
     normals.at(indices[i*3+1]) += m2.bv_normals().at(i);
     normals.at(indices[i*3+2]) += m2.bv_normals().at(i);
  }

  // ... so we have to calculate an average normal vector to use them with indices
  for (Points_t::iterator norm = normals.begin(); norm != normals.end(); ++norm) {
     *norm = glm::normalize(*norm);
  }

  InsertIndices(indices, vertices_.size(), &indices_);
  normals_.insert(normals_.end(), normals.begin(), normals.end());
  vertices_.insert(vertices_.end(), vertices.begin(), vertices.end());
}

void Adt_c::TransformVertices(const glm::vec3 &translation,
                              const glm::vec3 &rotation,
                              float scale,
                              Points_t *vertices) {
  glm::mat4 rot, rot_x, rot_y, rot_z, emp;

  //rot = glm::rotate(emp, -90.0f, glm::vec3(1, 0, 0));
  rot_x = glm::rotate(emp, rotation.z, glm::vec3(1, 0, 0));
  rot_z = glm::rotate(emp, -rotation.x, glm::vec3(0, 0, 1));
  rot_y = glm::rotate(emp, rotation.y-90, glm::vec3(0, 1, 0));
  rot = rot_y * rot_z * rot_x;

  // transform vertices
  for (Points_t::iterator vtx = vertices->begin();
       vtx != vertices->end();
       ++vtx) {
    glm::vec4 vtx4(vtx->x, vtx->z, -vtx->y, 1);   // important: X Z -Y !!!
    vtx4 = scale * rot * vtx4;

    *vtx = glm::vec3(vtx4) + translation;
  }
}

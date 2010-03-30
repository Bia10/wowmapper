#include "adt_c.h"

Adt_c::Adt_c(const uint8_t *buffer, uint32_t length, MpqHandler_c &mpq_h)
    : Chunk_c(buffer, length),
      mhdr_(this), mmdx_(this), mmid_(this), mwmo_(this),
      mwid_(this), mddf_(this), modf_(this), mh2o_(this) {
  GetSubChunk("RDHM", 4, &mhdr_);
  GetSubChunk("XDMM", 4, &mmdx_);
  GetSubChunk("DIMM", 4, &mmid_);
  GetSubChunk("OMWM", 4, &mwmo_);
  GetSubChunk("DIWM", 4, &mwid_);
  GetSubChunk("FDDM", 4, &mddf_);
  GetSubChunk("FDOM", 4, &modf_);
  GetSubChunk("O2HM", 4, &mh2o_);

  GenerateTerrain();
  InsertDoodads(mpq_h);

  for (int tile_y = 0; tile_y < 16; tile_y++) {
    for (int tile_x = 0; tile_x < 16; tile_x++) {
      int cur_tile = tile_y*16+tile_x;
      if (mh2o_.heights[cur_tile].val.size()) {
        Mh2oChunk_s::Mh2oHeights_s &heightmap = mh2o_.heights[cur_tile];
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

Adt_c::~Adt_c() {
  for (DoodadMap_t::iterator dd = doodads_.begin(); dd != doodads_.end(); ++dd) {
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

void Adt_c::InsertDoodads(MpqHandler_c &mpq_h) {
  for (McinChunk_s::McnkChunks_t::iterator mcnk = mhdr_.mcin.mcnks.begin();
       mcnk != mhdr_.mcin.mcnks.end();
       ++mcnk) {
    // get doodad filenames
    Filenames_t filenames;
    for (Indices32_t::iterator offset = mcnk->mcrf.doodad_offsets.begin();
         offset != mcnk->mcrf.doodad_offsets.end();
         ++offset) {
      MddfChunk_s::DoodadInfo_s &info = mddf_.doodad_info.at(*offset);
      std::string filename(mmdx_.m2_names.c_str() + mmid_.name_offsets.at(info.id));

      // replace false extension with right one
      std::string m2_name = filename;
      m2_name.replace(m2_name.find(".mdx", 0), 4, ".m2");

      // look for already loaded m2 and use them instead of reloading them
      DoodadMap_t::iterator found = doodads_.find(m2_name);
      if(found != doodads_.end()) {
        InsertBoundingTriangles(*(*found).second, info);
        continue;
      }

      // load m2 model file
      uint8_t *m2_buf = NULL;
      int64_t m2_size = mpq_h.LoadFileByName(m2_name.c_str(), &m2_buf);
      std::cout << "Load M2: " << m2_name << std::endl;

      M2_c *m2 = new M2_c(m2_buf, m2_size);
      doodads_.insert(DoodadPair_t(m2_name, m2));

      //InsertBoundingTriangles(*m2, info);

      // load m2 skin file
      /*std::string skin_name = filename;
      skin_name.replace(skin_name.find(".mdx", 0), 4, "00.skin");
      uint8_t *skin_buf = NULL;
      int64_t skin_size = mpq_h.LoadFileByName(skin_name.c_str(), &skin_buf);
      std::cout << "Load Skin: " << skin_name << std::endl;
      Skin_c skin(skin_buf, skin_size);

      InsertVisibleTriangles(m2, skin, info)*/
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

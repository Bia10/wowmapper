#include "m2_c.h"

M2_c::M2_c(Buffer_t *buffer) : Model_c(buffer), skin(NULL) {
  // get content information and resize to fit
  BlockInfo_s content_info = GetField<BlockInfo_s>(0x3c);
  content_.resize(content_info.num);
  // copy buffer and insert content
  CopyDataBlock(buffer_, content_info.offset, content_info.num, &content_);
}

M2_c::~M2_c() {
  delete skin; skin = NULL;
}

void M2_c::GetMesh(const Skin_c &skin, Mesh_s *mesh) const {
  skin.GetIndices(&mesh->idx);
  GetVertices(&mesh->vtx);
  GetNormals(&mesh->norm);
  mesh->col.assign(mesh->vtx.size(), 0xff00ffff);
}

void M2_c::GetBVMesh(Mesh_s *mesh) const {
  // bounding volume indices, vertices and normals info
  BlockInfo_s bvi_info = GetField<BlockInfo_s>(0xd8);
  BlockInfo_s bvv_info = GetField<BlockInfo_s>(0xe0);
  BlockInfo_s bvn_info = GetField<BlockInfo_s>(0xe8);

  //mesh->idx.resize(bvi_info.num);
  mesh->vtx.resize(bvv_info.num);
  mesh->norm.resize(bvv_info.num);
  Points_t norms(bvn_info.num);

  Indices16_t temp_idx(bvi_info.num);
  CopyDataBlock(buffer_, bvi_info.offset, bvi_info.num, &temp_idx);
  CopyDataBlock(buffer_, bvv_info.offset, bvv_info.num, &mesh->vtx);
  CopyDataBlock(buffer_, bvn_info.offset, bvn_info.num, &norms);

  mesh->idx.assign(temp_idx.begin(), temp_idx.end());
  mesh->col.assign(bvv_info.num, 0xff00ffff);

  uint32_t tri_norm_count = bvi_info.num / 3;
  for (uint32_t i = 0; i < tri_norm_count; i++) {
    mesh->norm[mesh->idx[i*3+0]] += norms[i];
    mesh->norm[mesh->idx[i*3+1]] += norms[i];
    mesh->norm[mesh->idx[i*3+2]] += norms[i];
  }

  for (Points_t::iterator norm = mesh->norm.begin();
       norm != mesh->norm.end();
       ++norm) {
    *norm = glm::normalize(*norm);
  }
}

void M2_c::GetVertices(Points_t *vertices) const {
  vertices->reserve(content_.size());
  for (M2Geometry_t::const_iterator m2g = content_.begin();
       m2g != content_.end();
       ++m2g) {
    vertices->push_back(m2g->position);
  }
}

void M2_c::GetNormals(Points_t *normals) const {
  normals->reserve(content_.size());
  for (M2Geometry_t::const_iterator m2g = content_.begin();
       m2g != content_.end();
       ++m2g) {
    normals->push_back(m2g->normal);
  }
}

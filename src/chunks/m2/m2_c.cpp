#include "m2_c.h"

M2_c::M2_c(Buffer_t *buffer) : Chunk_c(buffer), skin(NULL) {
  // get content information and resize to fit
  BlockInfo_s content_info = GetField<BlockInfo_s>(0x3c);
  content_.resize(content_info.num);

  int32_t off = content_info.offset;
  size_t geo_size = sizeof(M2Geometry_s);
  Buffer_t::iterator buf_begin = buffer_.begin();

  // copy buffer and insert content
  Buffer_t content_buf(buf_begin+off, buf_begin+off+geo_size*content_info.num);
  CopyDataBlock(content_buf, &content_);
}

M2_c::~M2_c() {
  delete skin; skin = NULL;
}

void M2_c::GetMesh(const Skin_c &skin, Mesh_s *mesh) const {
  skin.GetIndices(&mesh->idx);
  GetVertices(&mesh->vtx);
  GetNormals(&mesh->norm);
}

void M2_c::GetBVMesh(Mesh_s *mesh) const {

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

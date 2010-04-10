#include "m2_c.h"

M2_c::M2_c(Buffer_t *buffer) : Chunk_c(buffer) {
  // retrieve information blocks
  BlockInfo_s vtx_info = GetField<BlockInfo_s>(0x3c);
  vertices_.resize(vtx_info.num);

  if (buffer_.size() < vtx_info.offset+sizeof(M2Vertex_s)*vtx_info.num) {
    std::cout << buffer_.size() << " " << sizeof(M2Vertex_s)*vtx_info.num;
  }

  Buffer_t vtx_buf(buffer_.begin()+vtx_info.offset,
      buffer_.begin()+vtx_info.offset+sizeof(M2Vertex_s)*vtx_info.num);
  CopyDataBlock(vtx_buf, &vertices_);

  BlockInfo_s bv_idx_info, bv_vtx_info, bv_norm_info;

  bv_idx_info = GetField<BlockInfo_s>(0xd8);
  bv_indices_.resize(bv_idx_info.num);
  bv_vtx_info = GetField<BlockInfo_s>(0xe0);
  bv_vertices_.resize(bv_vtx_info.num);
  bv_norm_info = GetField<BlockInfo_s>(0xe8);
  bv_normals_.resize(bv_norm_info.num);

  // prepare buffers to fill vectors with CopyDataBlock
  Buffer_t bv_idx_buf(buffer_.begin()+bv_idx_info.offset,
      buffer_.begin()+bv_idx_info.offset+sizeof(uint16_t)*bv_idx_info.num);
  Buffer_t bv_vtx_buf(buffer_.begin()+bv_vtx_info.offset,
      buffer_.begin()+bv_vtx_info.offset+sizeof(glm::vec3)*bv_vtx_info.num);
  Buffer_t bv_norm_buf(buffer_.begin()+bv_norm_info.offset,
      buffer_.begin()+bv_norm_info.offset+sizeof(glm::vec3)*bv_norm_info.num);

  CopyDataBlock(bv_idx_buf, &bv_indices_);
  CopyDataBlock(bv_vtx_buf, &bv_vertices_);
  CopyDataBlock(bv_norm_buf, &bv_normals_);
}

void M2_c::GetVertices(Points_t *buffer) const {
  buffer->reserve(vertices_.size());

  for (M2Vertices_t::const_iterator vtx = vertices_.begin();
       vtx != vertices_.end();
       ++vtx) {
    buffer->push_back(vtx->position);
  }
}

void M2_c::GetNormals(Points_t *buffer) const {
  buffer->reserve(vertices_.size());

  for (M2Vertices_t::const_iterator vtx = vertices_.begin();
       vtx != vertices_.end();
       ++vtx) {
    buffer->push_back(vtx->normal);
  }
}

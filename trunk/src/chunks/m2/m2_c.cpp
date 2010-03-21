#include "m2_c.h"

M2_c::M2_c(const uint8_t *buffer, uint32_t length)
    : Chunk_c(buffer, length) {
  // retrieve information blocks
  BlockInfo_s vertices = GetField<BlockInfo_s>(0x3c);
  vertices_.resize(vertices.num);
  BlockInfo_s bv_indices = GetField<BlockInfo_s>(0xd8);
  bv_indices_.resize(bv_indices.num);
  BlockInfo_s bv_vertices = GetField<BlockInfo_s>(0xe0);
  bv_vertices_.resize(bv_vertices.num);

  // prepare buffers to fill vectors with CopyDataBlock
  Buffer_t vertex_buffer(buffer_.begin()+vertices.offset,
      buffer_.begin()+vertices.offset+sizeof(M2Vertex_s)*vertices.num);
  Buffer_t bv_index_buffer(buffer_.begin()+bv_indices.offset,
      buffer_.begin()+bv_indices.offset+sizeof(uint16_t)*bv_indices.num);
  Buffer_t bv_vertex_buffer(buffer_.begin()+bv_vertices.offset,
        buffer_.begin()+bv_vertices.offset+sizeof(glm::vec3)*bv_vertices.num);

  CopyDataBlock(vertex_buffer, &vertices_);
  CopyDataBlock(bv_index_buffer, &bv_indices_);
  CopyDataBlock(bv_vertex_buffer, &bv_vertices_);
}

void M2_c::get_vertex_buffer(Points_t *buffer) const {
  buffer->reserve(vertices_.size());

  for (M2Vertices_t::const_iterator vtx = vertices_.begin();
       vtx != vertices_.end();
       ++vtx) {
    buffer->push_back(glm::vec3(vtx->position.x, vtx->position.z, vtx->position.y));
  }
}

void M2_c::get_normal_buffer(Points_t *buffer) const {
  buffer->reserve(vertices_.size());

  for (M2Vertices_t::const_iterator vtx = vertices_.begin();
       vtx != vertices_.end();
       ++vtx) {
    buffer->push_back(vtx->normal);
  }
}

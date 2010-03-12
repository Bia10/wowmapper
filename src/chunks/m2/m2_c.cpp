#include "m2_c.h"

struct BlockInfo_s {
  uint32_t num;
  uint32_t offset;
};

M2_c::M2_c(const uint8_t *buffer, uint32_t length)
    : Chunk_c(buffer, length) {
  BlockInfo_s vertices = GetField<BlockInfo_s>(0x3c);
  m2_vertices_.resize(vertices.num);

  Buffer_t vertex_buffer(buffer_.begin()+vertices.offset,
      buffer_.begin()+vertices.offset+sizeof(M2Vertex_s)*vertices.num);

  CopyDataBlock(vertex_buffer, &m2_vertices_);
}



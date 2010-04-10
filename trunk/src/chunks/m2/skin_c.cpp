#include "skin_c.h"

Skin_c::Skin_c(Buffer_t *buffer) : Chunk_c(buffer) {
  // retrieve information blocks
  BlockInfo_s indices = GetField<BlockInfo_s>(0x04);
  indices_.resize(indices.num);
  BlockInfo_s triangles = GetField<BlockInfo_s>(0x0c);
  triangles_.resize(triangles.num);

  // prepare buffers to fill vectors with CopyDataBlock
  Buffer_t index_buffer(buffer_.begin()+indices.offset,
      buffer_.begin()+indices.offset+sizeof(uint16_t)*indices.num);
  Buffer_t triangle_buffer(buffer_.begin()+triangles.offset,
        buffer_.begin()+triangles.offset+sizeof(uint16_t)*triangles.num);

  CopyDataBlock(index_buffer, &indices_);
  CopyDataBlock(triangle_buffer, &triangles_);
}

void Skin_c::GetIndices(Indices32_t *buffer) const {
  buffer->reserve(triangles_.size());

  try {
    for (Indices16_t::const_iterator idx = triangles_.begin();
         idx != triangles_.end();
         ++idx) {
      buffer->push_back(indices_.at(*idx));
    }
  } catch(std::exception &e) {
    std::cout << __LINE__ << " " << __FILE__ << ": " << e.what() << std::endl;
    buffer->clear(); // clear buffer if anything went wrong
    return;
  }
}

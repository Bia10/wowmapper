#include "skin_c.h"

Skin_c::Skin_c(Buffer_t *buffer) : Chunk_c(buffer) {
  // retrieve information blocks
  BlockInfo_s idx_info = GetField<BlockInfo_s>(0x04);
  indices_.resize(idx_info.num);
  BlockInfo_s tri_info = GetField<BlockInfo_s>(0x0c);
  triangles_.resize(tri_info.num);

  CopyDataBlock(buffer_, idx_info.offset, idx_info.num, &indices_);
  CopyDataBlock(buffer_, tri_info.offset, tri_info.num, &triangles_);
}

void Skin_c::GetIndices(Indices32_t *buffer) const {
  buffer->reserve(triangles_.size());

  try {
    for (Indices16_t::const_iterator tri = triangles_.begin();
         tri != triangles_.end();
         ++tri) {
      buffer->push_back(indices_.at(*tri));
    }
  } catch(std::exception &e) {
    std::cout << __FILE__ << " - GetIndices: " << e.what() << std::endl;
    buffer->clear(); // clear buffer if anything went wrong
    return;
  }
}

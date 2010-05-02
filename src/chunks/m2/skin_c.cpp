#include "skin_c.h"

Skin_c::Skin_c(Buffer_t *buffer) : Chunk_c(buffer) {
  // retrieve information blocks
  BlockInfo_s idx_info = GetValue<BlockInfo_s>(0x04, 0);
  indices_.resize(idx_info.num);
  BlockInfo_s tri_info = GetValue<BlockInfo_s>(0x0c, 0);
  triangles_.resize(tri_info.num);

  CopyVector(GetBuffer(), idx_info.offset, idx_info.num, &indices_);
  CopyVector(GetBuffer(), tri_info.offset, tri_info.num, &triangles_);
}

void Skin_c::GetIndices(Indices32_t *buffer) const {
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

#include "mmidchunk_s.h"

MmidChunk_s::MmidChunk_s(Chunk_c *parent, off_t off)
  : Chunk_c(parent, off) {
  size_t num_off = GetSize() / sizeof(uint32_t);
  name_offs.resize(num_off);
  CopyVector(GetBuffer(), GetCurOffset()+DATA_OFFSET, num_off, &name_offs);
}

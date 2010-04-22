#include "modfchunk_s.h"

ModfChunk_s::ModfChunk_s(Chunk_c *parent, off_t off)
    : Chunk_c(parent, off) {
  size_t num_infos = GetSize() / sizeof(WmoInfo_s);
  wmo_infos.reserve(num_infos);
  CopyVector(GetBuffer(), GetCurOffset()+DATA_OFFSET, num_infos, &wmo_infos);
}

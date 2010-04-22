#include "mwmochunk_s.h"

MwmoChunk_s::MwmoChunk_s(Chunk_c *parent, off_t off)
  : Chunk_c(parent, off) {
  size_t name_size = GetSize();
  Buffer_t names(name_size);
  CopyVector(GetBuffer(), GetCurOffset()+DATA_OFFSET, name_size, &names);
  wmo_names.assign(names.begin(), names.end());
}

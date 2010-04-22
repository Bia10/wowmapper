#include "mcinchunk_s.h"

McinChunk_s::McinChunk_s(Chunk_c *parent, off_t off)
    : Chunk_c(parent, off), mcnk_info(256) {
  CopyVector(GetBuffer(), GetCurOffset()+DATA_OFFSET, 256, &mcnk_info);
}

#pragma once

#include "../chunk_s.h"

/*! @brief MPHD chunk. */
struct MphdChunk_s : Chunk_s {
  uint32_t flags;
  uint32_t something;
  uint32_t unused[6];

  MphdChunk_s(uint32_t offset, void *buffer) : Chunk_s(offset, buffer, true) { }
};

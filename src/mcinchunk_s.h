#pragma once

#include "chunk_s.h"

struct McinChunk_s : Chunk_s {
  struct McnkIndex_s {
    void *mcnk;
    uint32_t size;
    uint32_t unused_flags;
    uint32_t unused_async_id;
  };

  McnkIndex_s mcnk_index[256];
};

#pragma once

#include "mcnkchunk_s.h"

/*! @brief MCIN chunk */
struct McinChunk_s : Chunk_s {
  struct McnkIndex_s {
    McnkChunk_s *mcnk;
    uint32_t size;
    uint32_t unused_flags;
    uint32_t unused_async_id;
  };

  McnkIndex_s mcnk_index[256];

  McinChunk_s(uint32_t offset, void *buffer) : Chunk_s(offset, buffer, true) {
    /* intialize sub chunks: MCNK */
    int32_t mcnk_offset;
    for(int i = 0; i < 256; i++) {
      mcnk_offset = reinterpret_cast<uint32_t>(mcnk_index[i].mcnk);
      mcnk_index[i].mcnk = new McnkChunk_s(mcnk_offset, buffer);
    }
  }

  ~McinChunk_s() {
    for(int i = 0; i < 256; i++) {
      SAFE_DELETE(mcnk_index[i].mcnk);
    }
  }
};

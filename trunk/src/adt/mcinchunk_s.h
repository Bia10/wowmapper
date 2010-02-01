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

  McinChunk_s(int32_t memAbsOffset, void *in_buf) {
    int32_t buf_addr = reinterpret_cast<int32_t>(in_buf);
    void *dest_addr = reinterpret_cast<void*>(buf_addr + memAbsOffset);

    /* fill MCVT chunk with its real content */
    memcpy(this, dest_addr, sizeof(McinChunk_s));

    /* intialize sub chunks: MCNK */
    int32_t mcnk_abs_offset;
    for(int i = 0; i < 256; i++) {
      mcnk_abs_offset = reinterpret_cast<int32_t>(mcnk_index[i].mcnk);
      mcnk_index[i].mcnk = new McnkChunk_s(mcnk_abs_offset, in_buf);
    }
  }

  ~McinChunk_s() {
    for(int i = 0; i < 256; i++) {
      SAFE_DELETE(mcnk_index[i].mcnk);
    }
  }
};

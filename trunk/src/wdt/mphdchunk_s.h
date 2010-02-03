#pragma once

#include "../chunk_s.h"

/*! @brief MPHD chunk. */
struct MphdChunk_s : Chunk_s {
  uint32_t flags;
  uint32_t something;
  uint32_t unused[6];

  MphdChunk_s(int32_t memAbsOffset, void *in_buf) {
    int32_t buf_addr = reinterpret_cast<int32_t>(in_buf);
    void *dest_addr = reinterpret_cast<void*>(buf_addr + memAbsOffset);

    /* fill MPHD chunk with its real content */
    memcpy(this, dest_addr, sizeof(MphdChunk_s));
  }
};

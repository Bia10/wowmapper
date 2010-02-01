#pragma once

#include "../chunk_s.h"

/*! @brief MCVT chunk, 9x9 with an inner 8x8 field: 1-9, 10-17, 18-26, ... */
struct McvtChunk_s : Chunk_s {
  float heights[145];

  McvtChunk_s(int32_t memAbsOffset, void *in_buf) {
    int32_t buf_addr = reinterpret_cast<int32_t>(in_buf);
    void *dest_addr = reinterpret_cast<void*>(buf_addr + memAbsOffset);

    /* fill MCVT chunk with its real content */
    memcpy(this, dest_addr, sizeof(McvtChunk_s));
  }
};

#pragma once

#include "../chunk_s.h"

struct McnrChunk_s : Chunk_s {
  struct Normal_s {
    float x() const { return _x / 127.0f; }
    float y() const { return _y / 127.0f; }
    float z() const { return _z / 127.0f; }

   private:
    int8_t _x, _z, _y;
  };

  Normal_s normals[145];

  McnrChunk_s(int32_t memAbsOffset, void *in_buf) {
    int32_t buf_addr = reinterpret_cast<int32_t>(in_buf);
    void *dest_addr = reinterpret_cast<void*>(buf_addr + memAbsOffset);

    /* fill MCNR chunk with its real content */
    memcpy(this, dest_addr, sizeof(McnrChunk_s));
  }
};

#pragma once

#include "../chunk_s.h"

struct MognChunk_s : Chunk_s {
  char *group_names; //!< zero-terminated strings, names purely informational

  MognChunk_s(uint32_t offset, void *buffer)
      : Chunk_s(offset, buffer, false),
        group_names(NULL) {
    /* copy group names if size > 0 */
    if(size > 0) {
      uint32_t buf_addr = reinterpret_cast<uint32_t>(buffer) + offset;
      group_names = new char[size];
      void *src_addr = reinterpret_cast<void*>(buf_addr + sizeof(Chunk_s));
      memcpy(group_names, src_addr, size);
    }
  }

  ~MognChunk_s() {
    delete [] group_names;
  }
};

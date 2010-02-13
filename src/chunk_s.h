#pragma once

#include <string.h>
#include <stdint.h>
#include <iostream>
#include "common.h"

/*! @brief Base chunk structure for all chunks. */
struct Chunk_s {
  char id[4];
  uint32_t size;

  uint32_t total_size() const { return 0x8 + size; }

  Chunk_s() {}
  Chunk_s(int32_t offset, void *buffer, bool copyData) {
    /* copy base chunk content */
    uint32_t buf_addr = reinterpret_cast<uint32_t>(buffer);
    void *src_addr = reinterpret_cast<void*>(buf_addr + offset);
    memcpy(this, src_addr, sizeof(Chunk_s));

    if(copyData) {
      /* copy chunk data */
      uint32_t this_addr = reinterpret_cast<uint32_t>(this);
      void *dest_addr = reinterpret_cast<void*>(this_addr + sizeof(Chunk_s));
      src_addr = reinterpret_cast<void*>(buf_addr + offset + sizeof(Chunk_s));
      memcpy(dest_addr, src_addr, size);
    }

    std::cout << id[3] << id[2] << id[1] << id[0] << " chunk initialized: ";
    std::cout << total_size() << " bytes" << std::endl;
  }
};

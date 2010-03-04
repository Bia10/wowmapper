#pragma once

#include "../chunk_s.h"

struct ModnChunk_s : Chunk_s {
  char *m2_filnames;

  ModnChunk_s() : Chunk_s() {
    /* copy MODN chunk manually */
    /*uint32_t buf_addr = reinterpret_cast<uint32_t>(buffer);
    void *src_addr = reinterpret_cast<void*>(buf_addr + offset + sizeof(Chunk_s));

    m2_filnames = new char[size];
    memcpy(m2_filnames, src_addr, size);*/
  }

  ~ModnChunk_s() {
    delete [] m2_filnames;
  }
};

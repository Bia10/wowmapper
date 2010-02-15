#pragma once

#include "../chunk_s.h"

struct MoviChunk_s : Chunk_s {
  Indices16_t indices;

  MoviChunk_s(uint32_t offset, void *buffer) : Chunk_s(offset, buffer, false) {
    /* copy MOVI chunk manually to avoid memory corruption */
    uint32_t buf_addr = reinterpret_cast<uint32_t>(buffer);
    void *src_addr = reinterpret_cast<void*>(buf_addr + offset + sizeof(Chunk_s));

    /* copy vertex indices to array */
    indices.resize(size / sizeof(uint16_t));
    memcpy(&indices[0], src_addr, size);
  }
};

typedef std::auto_ptr<MoviChunk_s> MoviChunk_t;

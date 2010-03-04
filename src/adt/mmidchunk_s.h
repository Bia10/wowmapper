#pragma once

#include "../chunk_s.h"

/*! \brief MMID chunk. */
struct MmidChunk_s : Chunk_s {
  Indices32_t indices;

  virtual void Initialize() {
    indices.resize(size / sizeof(uint32_t));
    CopyData(indices.data());
  }
};

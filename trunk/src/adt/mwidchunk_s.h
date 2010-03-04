#pragma once

#include "../chunk_s.h"

/*! \brief MWID chunk. */
struct MwidChunk_s : Chunk_s {
  Indices32_t indices;

  virtual void Initialize() {
    indices.resize(size / sizeof(uint32_t));
    CopyData(indices.data());
  }
};

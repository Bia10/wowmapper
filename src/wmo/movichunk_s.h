#pragma once

#include "../chunk_s.h"
#include "mopychunk_s.h"

/*! \brief MOVI chunk. */
struct MoviChunk_s : Chunk_s {
  Indices16_t indices;

  virtual void Initialize() {
    indices.resize(size / sizeof(uint16_t));
    CopyData(indices.data());
  }
};

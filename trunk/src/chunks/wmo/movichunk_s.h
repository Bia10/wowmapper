#pragma once

#include "../chunk_c.h"

/*! \brief MOVI: Map Object Vertex Indices. */
struct MoviChunk_s : public Chunk_c {
  Indices16_t indices;

  MoviChunk_s(Chunk_c *parent, wm_off_t off)
        : Chunk_c(parent, off) {
      wm_size_t num_indices = GetSize() / sizeof(uint16_t);
      indices.resize(num_indices);
      CopyVector(GetBuffer(), GetCurOffset()+DATA_OFFSET, num_indices, &indices);
    }
};

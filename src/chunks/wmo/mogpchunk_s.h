#pragma once

#include "../chunk_c.h"

/*! \brief MOGP: Map Object Group Information. */
struct MogpChunk_s : public Chunk_c {

  MogpChunk_s(Chunk_c *parent, off_t off)
      : Chunk_c(parent, off) {

  }
};

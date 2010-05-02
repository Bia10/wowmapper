#pragma once

#include "../chunk_c.h"

struct MoltChunk_s : public Chunk_c {
  MoltChunk_s(Chunk_c *parent, off_t off)
      : Chunk_c(parent, off) {

  }
};

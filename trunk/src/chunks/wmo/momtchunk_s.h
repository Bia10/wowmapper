#pragma once

#include "../chunk_c.h"

struct MomtChunk_s : public Chunk_c {
  MomtChunk_s(Chunk_c *parent, off_t off)
      : Chunk_c(parent, off) {

  }
};

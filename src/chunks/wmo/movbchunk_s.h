#pragma once

#include "../chunk_c.h"

struct MovbChunk_s : public Chunk_c {
  MovbChunk_s(Chunk_c *parent, off_t off)
      : Chunk_c(parent, off) {

  }
};

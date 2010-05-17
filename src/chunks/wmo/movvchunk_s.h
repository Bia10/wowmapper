#pragma once

#include "../chunk_c.h"

struct MovvChunk_s : public Chunk_c {
  MovvChunk_s(Chunk_c *parent, wm_off_t off)
      : Chunk_c(parent, off) {

  }
};

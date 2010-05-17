#pragma once

#include "../chunk_c.h"

struct MoprChunk_s : public Chunk_c {
  MoprChunk_s(Chunk_c *parent, wm_off_t off)
      : Chunk_c(parent, off) {

  }
};

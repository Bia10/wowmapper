#pragma once

#include "../chunk_c.h"

struct MosbChunk_s : public Chunk_c {
  MosbChunk_s(Chunk_c *parent, wm_off_t off)
      : Chunk_c(parent, off) {

  }
};

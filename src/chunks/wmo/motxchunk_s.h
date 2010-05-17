#pragma once

#include "../chunk_c.h"

struct MotxChunk_s : public Chunk_c {
  MotxChunk_s(Chunk_c *parent, wm_off_t off)
      : Chunk_c(parent, off) {

  }
};

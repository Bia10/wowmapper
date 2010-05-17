#pragma once

#include "../chunk_c.h"

struct MopvChunk_s : public Chunk_c {
  MopvChunk_s(Chunk_c *parent, wm_off_t off)
      : Chunk_c(parent, off) {

  }
};

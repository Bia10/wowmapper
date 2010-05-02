#pragma once

#include "../chunk_c.h"

/*! \brief MPHD chunk. */
struct MphdChunk_s : public Chunk_c {
  uint32_t flags;


  MphdChunk_s(Chunk_c *parent, off_t off)
      : Chunk_c(parent, off),
        flags(GetValue<uint32_t>(0x8)) {

  }
};

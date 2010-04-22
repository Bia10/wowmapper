#pragma once

#include "../chunk_c.h"

/*! \brief MMID: Map Chunk M2 Name Offsets. */
struct MmidChunk_s : public Chunk_c {
  Indices32_t name_offs;

  MmidChunk_s(Chunk_c *parent, off_t off);
};

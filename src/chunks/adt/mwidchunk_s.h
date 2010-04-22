#pragma once

#include "../chunk_c.h"

/*! \brief MWID: Map Chunk WMO Name Offsets. */
struct MwidChunk_s : public Chunk_c {
  Indices32_t name_offs;

  MwidChunk_s(Chunk_c *parent, off_t off);
};

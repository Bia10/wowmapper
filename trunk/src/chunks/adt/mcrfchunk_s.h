#pragma once

#include "../chunk_c.h"

/*! \brief MCRF: Map Chunk references. */
struct McrfChunk_s : public Chunk_c {
  Indices32_t doodad_offs;
  Indices32_t wmo_offs;

  McrfChunk_s(Chunk_c *parent, off_t off);
};

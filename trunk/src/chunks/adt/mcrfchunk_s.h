#pragma once

#include "../chunk_c.h"

/*! \brief MCRF: Map Chunk references. */
struct McrfChunk_s : public Chunk_c {
  Indices32_t doodad_offs;
  Indices32_t wmo_offs;


  McrfChunk_s(Chunk_c *parent, off_t off, size_t num_doodads, size_t num_wmos)
      : Chunk_c(parent, off) {
    off_t ref_off = GetCurOffset() + DATA_OFFSET;
    off_t wmo_off = ref_off+num_doodads*sizeof(uint32_t);

    doodad_offs.resize(num_doodads);
    wmo_offs.resize(num_wmos);

    CopyVector(GetBuffer(), ref_off, num_doodads, &doodad_offs);
    CopyVector(GetBuffer(), wmo_off, num_wmos, &wmo_offs);
  }
};

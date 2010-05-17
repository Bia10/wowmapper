#pragma once

#include "../chunk_c.h"

/*! \brief MWID: Map Chunk WMO Name Offsets. */
struct MwidChunk_s : public Chunk_c {
  Indices32_t name_offs;


  MwidChunk_s(Chunk_c *parent, wm_off_t off)
      : Chunk_c(parent, off) {
    wm_size_t num_off = GetSize() / sizeof(uint32_t);
    name_offs.resize(num_off);
    CopyVector(GetBuffer(), GetCurOffset()+DATA_OFFSET, num_off, &name_offs);
  }
};

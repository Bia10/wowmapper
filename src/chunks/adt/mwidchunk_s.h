#pragma once

#include "../chunk_c.h"

/*! \brief MWID: Map Chunk WMO Name Offsets. */
struct MwidChunk_s : public Chunk_c {
  Indices32_t name_offsets;

  MwidChunk_s(Chunk_c *parent) : Chunk_c(parent) { }

 protected:
  virtual void LateInit() {
    name_offsets.resize(buffer_.size()/sizeof(uint32_t));
    CopyDataBlock(buffer_, &name_offsets);
  }
};

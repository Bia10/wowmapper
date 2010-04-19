#pragma once

#include "../chunk_c.h"

/*! \brief MMID: Map Chunk M2 Name Offsets. */
struct MmidChunk_s : public Chunk_c {
  Indices32_t name_offsets;

  MmidChunk_s(Chunk_c *parent) : Chunk_c(parent) { }

 protected:
  virtual void LateInit() {
    name_offsets.resize(buffer_.size()/sizeof(uint32_t));
    CopyDataBlock(buffer_, &name_offsets);
  }
};

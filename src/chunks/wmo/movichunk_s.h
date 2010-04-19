#pragma once

#include "../chunk_c.h"

/*! \brief MOVI: Map Object Vertex Indices. */
struct MoviChunk_s : public Chunk_c {
  Indices16_t indices;

  MoviChunk_s(Chunk_c *parent) : Chunk_c(parent) { }

 protected:
  virtual void LateInit() {
    indices.resize(buffer_.size()/sizeof(uint16_t));
    CopyDataBlock(buffer_, &indices);
  }
};

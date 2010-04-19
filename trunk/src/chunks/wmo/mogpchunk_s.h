#pragma once

#include "../chunk_c.h"

/*! \brief MOGP: Map Object Group Information. */
struct MogpChunk_s : public Chunk_c {

  MogpChunk_s(Chunk_c *parent) : Chunk_c(parent) {}

 protected:
  virtual void LateInit() {

  }
};

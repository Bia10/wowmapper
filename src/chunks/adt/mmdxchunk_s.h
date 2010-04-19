#pragma once

#include "../chunk_c.h"

/*! \brief MMDX: Map Chunk M2 Names. */
struct MmdxChunk_s : public Chunk_c {
  std::string m2_names;

  MmdxChunk_s(Chunk_c *parent) : Chunk_c(parent) { }

 protected:
  virtual void LateInit() {
    m2_names.assign(buffer_.begin(), buffer_.end());
  }
};

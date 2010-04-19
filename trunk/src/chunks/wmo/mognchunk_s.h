#pragma once

#include "../chunk_c.h"

/*! \brief MOGN: Map Object Group Names. */
struct MognChunk_s : public Chunk_c {
  std::string group_names;

  MognChunk_s(Chunk_c *parent) : Chunk_c(parent) { }

 protected:
  virtual void LateInit() {
    group_names.assign(buffer_.begin(), buffer_.end());
  }
};

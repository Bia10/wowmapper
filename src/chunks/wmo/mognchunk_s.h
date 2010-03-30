#pragma once

#include "../chunk_c.h"

struct MognChunk_s : public Chunk_c {
  std::string group_names;

  MognChunk_s(Chunk_c *parent) : Chunk_c(parent) { }

 protected:
  virtual void Initialize() {
    group_names.assign(buffer_.begin(), buffer_.end());
  }
};

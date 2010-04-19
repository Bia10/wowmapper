#pragma once

#include "chunk_c.h"

class Model_c : public Chunk_c {
 public:
  Model_c(Buffer_t *buffer) : Chunk_c(buffer) {}
  virtual ~Model_c() {}
};

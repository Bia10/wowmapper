#pragma once

#include "../chunk_c.h"

struct MonrChunk_s : public Chunk_c {
  Points_t normals;

  MonrChunk_s(Chunk_c *parent) : Chunk_c(parent) { }

 protected:
  virtual void LateInit() {
    normals.resize(buffer_.size()/sizeof(glm::vec3));
    CopyDataBlock(buffer_, &normals);
  }
};

#pragma once

#include "../chunk_c.h"

/*! \brief MOVT: Map Object Vertices. */
struct MovtChunk_s : public Chunk_c {
  Points_t vertices;

  MovtChunk_s(Chunk_c *parent) : Chunk_c(parent) {}

 protected:
  virtual void LateInit() {
    vertices.resize(buffer_.size()/sizeof(glm::vec3));
    CopyDataBlock(buffer_, &vertices);
  }
};

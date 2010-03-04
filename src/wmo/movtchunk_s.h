#pragma once

#include "../chunk_s.h"

/*! @brief MOVT chunk. */
struct MovtChunk_s : Chunk_s {
  Points_t vertices;

  virtual void Initialize() {
    vertices.resize(size / sizeof(glm::vec3));
    CopyData(vertices.data());
  }
};

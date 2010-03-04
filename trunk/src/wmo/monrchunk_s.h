#pragma once

#include "../chunk_s.h"

/*! @brief MONR chunk. */
struct MonrChunk_s : Chunk_s {
  Points_t normals;

  MonrChunk_s() : Chunk_s() {

  }

  virtual void Initialize() {
    normals.resize(size / sizeof(glm::vec3));
    CopyData(normals.data());
  }
};

typedef std::auto_ptr<MonrChunk_s> MonrChunk_t;

#pragma once

#include "../chunk_c.h"

/*! \brief MONR: Map Object Normals. */
struct MonrChunk_s : public Chunk_c {
  Normals_t normals;

  MonrChunk_s(Chunk_c *parent, off_t off)
      : Chunk_c(parent, off) {
    size_t num_normals = GetSize() / sizeof(glm::vec3);
    normals.resize(num_normals);
    CopyVector(GetBuffer(), GetCurOffset()+DATA_OFFSET, num_normals, &normals);
  }
};

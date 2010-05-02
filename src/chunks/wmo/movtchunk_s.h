#pragma once

#include "../chunk_c.h"

/*! \brief MOVT: Map Object Vertices. */
struct MovtChunk_s : public Chunk_c {
  Vertices_t vertices;

  MovtChunk_s(Chunk_c *parent, off_t off)
      : Chunk_c(parent, off) {
    size_t num_vertices = GetSize() / sizeof(glm::vec3);
    vertices.resize(num_vertices);
    CopyVector(GetBuffer(), GetCurOffset()+DATA_OFFSET, num_vertices, &vertices);
  }
};

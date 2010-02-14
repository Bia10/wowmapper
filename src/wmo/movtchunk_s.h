#pragma once

#include "../chunk_s.h"

/*! @brief MOVT chunk. */
struct MovtChunk_s : Chunk_s {
  Points_t vertices;

  MovtChunk_s(uint32_t offset, void *buffer) : Chunk_s(offset, buffer, false) {
    /* copy MOVT chunk manually to avoid memory corruption */
    uint32_t buf_addr = reinterpret_cast<uint32_t>(buffer);
    void *src_addr = reinterpret_cast<void*>(buf_addr + offset + sizeof(Chunk_s));

    /* copy vertex indices to array */
    vertices.resize(size / sizeof(glm::vec3));
    memcpy(&vertices[0], src_addr, size);

    /* vertices are stored in X, Z, -Y order so we have to resort them */
    float temp;
    for(uint32_t i = 0; i < vertices.size(); i++) {
      temp = vertices[i].y;
      vertices[i].y = vertices[i].z;
      vertices[i].z = -temp;
    }
  }
};

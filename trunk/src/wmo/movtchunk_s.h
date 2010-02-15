#pragma once

#include "../chunk_s.h"
#include "mopychunk_s.h"

/*! @brief MOVT chunk. */
struct MovtChunk_s : Chunk_s {
  Points_t vertices;

  MovtChunk_s(uint32_t offset, void *buffer, const MopyChunk_t &mopyChunk)
      : Chunk_s(offset, buffer, false) {
    /* copy MOVT chunk manually*/
    uint32_t buf_addr = reinterpret_cast<uint32_t>(buffer);
    void *src_addr = reinterpret_cast<void*>(buf_addr + offset + sizeof(Chunk_s));

    /* copy vertex indices to array */
    vertices.resize(size / sizeof(glm::vec3));
    memcpy(&vertices[0], src_addr, size);

    /* vertices are stored in X, Z, -Y order so we have to change axes */
    std::transform(vertices.begin(), vertices.end(),
                   vertices.begin(), SwapAxes_ftor());
  }
};

typedef std::auto_ptr<MovtChunk_s> MovtChunk_t;

#pragma once

#include "../chunk_s.h"

/*! @brief MONR chunk. */
struct MonrChunk_s : Chunk_s {
  Points_t normals;

  MonrChunk_s(uint32_t offset, void *buffer) : Chunk_s(offset, buffer, false) {
    /* copy MONR chunk manually to avoid memory corruption */
    uint32_t buf_addr = reinterpret_cast<uint32_t>(buffer);
    void *src_addr = reinterpret_cast<void*>(buf_addr + offset + sizeof(Chunk_s));

    /* copy vertex indices to array */
    normals.resize(size / sizeof(glm::vec3));
    memcpy(&normals[0], src_addr, size);

    /* normals are stored in X, Z, -Y order so we have to change axes */
    std::transform(normals.begin(), normals.end(),
                   normals.begin(), SwapAxes_ftor());
  }
};

typedef std::auto_ptr<MonrChunk_s> MonrChunk_t;

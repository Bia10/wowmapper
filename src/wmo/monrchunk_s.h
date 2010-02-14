#pragma once

#include "../chunk_s.h"

struct SwapNormals_ftor {
  glm::vec3 operator()(glm::vec3 &value) {
      float temp = value.y;
      value.y = value.z;
      value.z = -temp;
      return value;
  }
};

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

    std::transform(normals.begin(), normals.end(), normals.begin(), SwapNormals_ftor());
  }
};

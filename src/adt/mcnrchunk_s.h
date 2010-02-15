#pragma once

#include "../chunk_s.h"

/*! @brief MCNR chunk. */
struct McnrChunk_s : Chunk_s {
 private:
  struct Normal_s { int8_t x, z, y; } normals_[145]; //<! normal (x, Z, y)

 public:
  Points_t normals;

  McnrChunk_s(uint32_t offset, void *buffer)
      : Chunk_s(offset, buffer, true),
        normals(145) {
    GenerateNormals();
  }

 private:
  void GenerateNormals() {
    for (int i = 0; i < 145; i++) {
      normals[i].x = normals_[i].x / 127.0f;
      normals[i].y = normals_[i].y / 127.0f;
      normals[i].z = normals_[i].z / 127.0f;
    }
  }
};

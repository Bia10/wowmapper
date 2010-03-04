#pragma once

#include "../chunk_s.h"

typedef struct { uint8_t x, y, z; } Normal_t;
typedef std::vector<Normal_t> RawNormals_t;

// functions is needed to make normals with floats from normals with chars
static glm::vec3 MakeNormals(const Normal_t &norm, const glm::vec3 &vec) {
  return glm::vec3(norm.x, norm.y, norm.z) * 0.007874016f; // 1:127
}

/*! \brief MCNR chunk. */
struct McnrChunk_s : Chunk_s {
 public:
  Points_t normals;


  McnrChunk_s() : Chunk_s(), normals(145) { }

  virtual void Initialize() {
    RawNormals_t raw_normals(145);
    CopyData(raw_normals.data());

    // after coping the raw data create normals with floats from it
    std::transform(raw_normals.begin(), raw_normals.end(),
        normals.begin(), normals.begin(), MakeNormals);

    // swap axes
    std::transform(normals.begin(), normals.end(), normals.begin(), SwapAxes);
  }
};

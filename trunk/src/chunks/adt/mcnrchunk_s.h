#pragma once

#include "../chunk_c.h"

static float NormalsI8toF(int8_t a) {
  return a * 0.007874016f; // 1/127.0f
}

/*! \brief MCNR chunk.
 *  http://www.madx.dk/wowdev/wiki/index.php?title=ADT/v18#MCNR_sub-chunk */
struct McnrChunk_s : public Chunk_c {
  std::vector<float> normals;

  McnrChunk_s(Chunk_c *parent) : Chunk_c(parent), normals(435) {}

  void GetNormals(Points_t *buffer) {
    buffer->reserve(145);
    for (int i = 0; i < 145; i++) {
      buffer->push_back(glm::vec3(normals.at(i*3+0),    // x
                                  normals.at(i*3+2),    // z
                                  normals.at(i*3+1)));  // y
    }
  }

 protected:
  virtual void Initialize() {
    Normals_t temp_normals(435);
    CopyDataBlock(buffer_, &temp_normals);
    std::transform(temp_normals.begin(), temp_normals.end(),
        normals.begin(), NormalsI8toF);
  }

 private:
  typedef std::vector<int8_t> Normals_t;
};

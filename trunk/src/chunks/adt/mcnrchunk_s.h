#pragma once

#include "../chunk_c.h"

static float NormalsI8toF(int8_t a) {
  return a * 0.007874016f; // a * (1/127)
}

/*! \brief MCNR: Map Chunk Normals. */
struct McnrChunk_s : public Chunk_c {
  std::vector<float> normals;


  McnrChunk_s(Chunk_c *parent, wm_off_t off)
      : Chunk_c(parent, off), normals(435) {
    // temporary normals array to hold these stupid 8 bit normals by blizz
    std::vector<int8_t> temp_normals(435);
    CopyVector(GetBuffer(), GetCurOffset()+DATA_OFFSET, 435, &temp_normals);
    // transform to float normals
    std::transform(temp_normals.begin(), temp_normals.end(),
        normals.begin(), NormalsI8toF);
  }

  void GetNormals(Vertices_t *norm) const {
    norm->reserve(145);
    for (int i = 0; i < 145; i++) {
      norm->push_back(glm::vec3(normals[i*3+0],    // x
                                normals[i*3+1],    // z
                                normals[i*3+2]));  // y
    }
  }
};






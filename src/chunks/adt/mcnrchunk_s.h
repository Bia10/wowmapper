#pragma once

#include "../chunk_c.h"

static float NormalsI8toF(int8_t a) {
  return a * 0.007874016f; // a * (1/127)
}

/*! \brief MCNR: Map Chunk Normals. */
struct McnrChunk_s : public Chunk_c {
  std::vector<float> normals;

  McnrChunk_s(Chunk_c *parent, off_t off);
  void GetNormals(Points_t *norm) const;
};

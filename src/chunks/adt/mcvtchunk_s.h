#pragma once

#include "../chunk_c.h"

/*! \brief MCVT: Map Chunk Vertices. */
struct McvtChunk_s : public Chunk_c {
  std::vector<float> heightmap;

  McvtChunk_s(Chunk_c *parent, off_t off);
  void GetVertices(Points_t *vertices) const;
  void GetIndices(Indices32_t *indices) const;
};

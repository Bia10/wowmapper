#include "mcvtchunk_s.h"
#include "mcnkchunk_s.h"

McvtChunk_s::McvtChunk_s(Chunk_c *parent, off_t off)
    : Chunk_c(parent, off), heightmap(145) {
  CopyVector(GetBuffer(), GetCurOffset()+DATA_OFFSET, 145, &heightmap);
}

void McvtChunk_s::GetVertices(Points_t *vertices) const {
  vertices->resize(145);
  const glm::vec3 &pos = reinterpret_cast<McnkChunk_s*>(parent_)->position;

  for (int i = 0; i < 145; i++) {
    uint32_t row_idx = i / 17;
    uint32_t col_idx = i % 17;

    bool isRow9x9 = col_idx < 9;
    if (isRow9x9) { // if it's a row of the outer 9x9 vector ...
      (*vertices)[i].x = col_idx*TU*2 - pos.y;
      (*vertices)[i].z = row_idx*TU*2 - pos.x;
    } else {        // if it's a row of the inner 8x8 vector ...
      (*vertices)[i].x = (col_idx-9)*TU*2+TU - pos.y;
      (*vertices)[i].z = row_idx*TU*2+TU - pos.x;
    }
    (*vertices)[i].y = heightmap[i] + pos.z;
  }
}

void McvtChunk_s::GetIndices(Indices32_t *indices) const {
  indices->resize(768);
  uint32_t holes = reinterpret_cast<McnkChunk_s*>(parent_)->holes;

  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 8; x++) {
      uint32_t i = y*8*12 + x*12;

      // filter holes
      if (!(holes & (1 << (y/2 * 4 + x/2)))) {
        // vertex 0
        (*indices)[i + 0] = y * 17 + x;      // top left
        (*indices)[i + 2] = y * 17 + x + 1;  // top right
        (*indices)[i + 1] = y * 17 + x + 9;  // center

        // vertex 1
        (*indices)[i + 3] = y * 17 + x + 1;  // top right
        (*indices)[i + 5] = y * 17 + x + 18; // bottom right
        (*indices)[i + 4] = y * 17 + x + 9;  // center

        // vertex 2
        (*indices)[i + 6] = y * 17 + x + 18; // bottom right
        (*indices)[i + 8] = y * 17 + x + 17; // bottom left
        (*indices)[i + 7] = y * 17 + x + 9;  // center

        // vertex 3
        (*indices)[i + 9] = y * 17 + x + 17; // bottom left
        (*indices)[i + 11] = y * 17 + x;     // top left
        (*indices)[i + 10] = y * 17 + x + 9; // center
      }
    }
  }
}

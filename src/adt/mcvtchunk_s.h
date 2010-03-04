#pragma once

#include "../chunk_s.h"

typedef std::vector<float> Heights_t;

/*! \brief MCVT chunk, 9x9 with an inner 8x8 field: 1-9, 10-17, 18-26, ... */
struct McvtChunk_s : Chunk_s {
  Heights_t heights;
  Points_t vertices;
  Indices16_t indices;


  McvtChunk_s() : Chunk_s(), heights(145), vertices(145), indices(768) { }

  virtual void Initialize() {
    CopyData(heights.data());
    GenerateVertices();
    GenerateIndices();
  }

 private:
  void GenerateVertices() {
    for (int i = 0; i < 145; i++) {
      bool isRow9 = (i % 17) < 9;
      uint32_t row_idx = i / 17;
      uint32_t col_idx = i % 17;

      if (isRow9) { // if it's a row of the outer 9x9 vector ...
        vertices[i].x = col_idx * UNIT * 2 - UNIT;
        vertices[i].z = row_idx * UNIT * 2 - UNIT;
      } else {      // if it's a row of the inner 8x8 vector ...
        vertices[i].x = (col_idx-9) * UNIT * 2;
        vertices[i].z = row_idx * UNIT * 2;
      }
      vertices[i].y = heights[i];
    }
  }

  void GenerateIndices() {
    uint32_t i = 0;
    for (int y = 0; y < 8; y++) {
      for (int x = 0; x < 8; x++) {
        i = y*8*12 + x*12;

        indices[i + 0] = y * 17 + x;      // top left
        indices[i + 1] = y * 17 + x + 1;  // top right
        indices[i + 2] = y * 17 + x + 9;  // center

        /* vertex 1 */
        indices[i + 3] = indices[i + 1];  // top right
        indices[i + 4] = y * 17 + x + 18; // bottom right
        indices[i + 5] = indices[i + 2];  // center

        /* vertex 2 */
        indices[i + 6] = indices[i + 4];  // bottom right
        indices[i + 7] = y * 17 + x + 17; // bottom left
        indices[i + 8] = indices[i + 2];  // center

        /* vertex 3 */
        indices[i + 9] = indices[i + 7];  // bottom left
        indices[i + 10] = indices[i + 0]; // top left
        indices[i + 11] = indices[i + 2]; // center
      }
    }
  }
};

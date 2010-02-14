#pragma once

#include "../chunk_s.h"

#define UNIT 2.083333333f // 100.0f / (3.0f * 16.0f);

/*! @brief MCVT chunk, 9x9 with an inner 8x8 field: 1-9, 10-17, 18-26, ... */
struct McvtChunk_s : Chunk_s {
  float heights[145];
  Points_t vertices;
  Indices32_t indices;

  /*! @brief Returns a tile patch of 8x8 patches.
     *
     * @param i Index
     * @return Returns an array of glm:vec3 containing 12 vertices.
     *         These 12 vertices present 4 triangles which make up the patch.
     */
  const glm::vec3* operator [](int32_t i) {
    int32_t col = i % 8;
    int32_t row = i / 8;

    float coord_x = col*2*UNIT;
    float coord_y = row*2*UNIT;

    float edge_left = coord_x - UNIT;
    float edge_right = coord_x + UNIT;
    float edge_top = coord_y - UNIT;
    float edge_bottom = coord_y + UNIT;

    float tl_h = heights[row*17+col];    // top-left height
    float tr_h = heights[row*17+col+1];  // top-right height
    float bl_h = heights[row*17+col+17]; // bottom-left height
    float br_h = heights[row*17+col+18]; // bottom-right height
    float ce_h = heights[row*17+col+9];  // center height

    static glm::vec3 vertices[12];
    /* top vertex (cw) */
    vertices[0] = glm::vec3(edge_left, tl_h, edge_top);
    vertices[1] = glm::vec3(edge_right, tr_h, edge_top);
    vertices[2] = glm::vec3(coord_x, ce_h, coord_y);
    /* right vertex (cw) */
    vertices[3] = glm::vec3(edge_right, tr_h, edge_top);
    vertices[4] = glm::vec3(edge_right, br_h, edge_bottom);
    vertices[5] = glm::vec3(coord_x, ce_h, coord_y);
    /* bottom vertex (cw) */
    vertices[6] = glm::vec3(edge_right, br_h, edge_bottom);
    vertices[7] = glm::vec3(edge_left, bl_h, edge_bottom);
    vertices[8] = glm::vec3(coord_x, ce_h, coord_y);
    /* left vertex (cw) */
    vertices[9] = glm::vec3(edge_left, bl_h, edge_bottom);
    vertices[10] = glm::vec3(edge_left, tl_h, edge_top);
    vertices[11] = glm::vec3(coord_x, ce_h, coord_y);

    return vertices;
  }

  McvtChunk_s(uint32_t offset, void *buffer)
      : Chunk_s(offset, buffer, true),
        vertices(145),
        indices(145*3) {
    /* generate vertices based on heights and indices */
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
    for (int y = 0; y < 8; y++) {
      for (int x = 0; x < 8; x++) {
        uint32_t i = y*8*3 + x*3;

        /* vertex 0 */
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
        indices[i + 6] = indices[i + 4];  // bottom left
        indices[i + 7] = indices[i + 0];  // top left
        indices[i + 8] = indices[i + 2];  // center
      }
    }
  }
};

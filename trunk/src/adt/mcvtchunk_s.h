#pragma once

#include "../chunk_s.h"

#define UNIT 2.083333333f // 100.0f / (3.0f * 16.0f);

/*! @brief MCVT chunk, 9x9 with an inner 8x8 field: 1-9, 10-17, 18-26, ... */
struct McvtChunk_s : Chunk_s {
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

    float tl_h = height[row*17+col];    // top-left height
    float tr_h = height[row*17+col+1];  // top-right height
    float bl_h = height[row*17+col+17]; // bottom-left height
    float br_h = height[row*17+col+18]; // bottom-right height
    float ce_h = height[row*17+col+9];  // center height

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

  McvtChunk_s(int32_t memAbsOffset, void *in_buf) {
    int32_t buf_addr = reinterpret_cast<int32_t>(in_buf);
    void *dest_addr = reinterpret_cast<void*>(buf_addr + memAbsOffset);

    /* fill MCVT chunk with its real content */
    memcpy(this, dest_addr, sizeof(McvtChunk_s));
  }

 private:
  float height[145];
};

#pragma once

#include "../chunk_s.h"

/*! @brief MCNR chunk. */
struct McnrChunk_s : Chunk_s {
  /*! @brief Returns a tile patch normals of 8x8 patches.
   *
   * @param i Index
   * @return Returns an array of glm:vec3 containing 12 normals.
   *         These 12 normals match the vertices from the MCVT chunk.
   */
  const glm::vec3* operator [](int32_t i) {
    int32_t col = i % 8;
    int32_t row = i / 8;

    Normal_s tl_n = normal[row*17+col];    // top-left normal
    Normal_s tr_n = normal[row*17+col+1];  // top-right normal
    Normal_s bl_n = normal[row*17+col+17]; // bottom-left normal
    Normal_s br_n = normal[row*17+col+18]; // bottom-right normal
    Normal_s ce_n = normal[row*17+col+9];  // center normal

    static glm::vec3 normals[12];
    /* top normals */
    normals[0] = glm::vec3(tl_n._x, tl_n._y, tl_n._z) / 127.0f;
    normals[1] = glm::vec3(tr_n._x, tr_n._y, tr_n._z) / 127.0f;
    normals[2] = glm::vec3(ce_n._x, ce_n._y, ce_n._z) / 127.0f;
    /* right normals */
    normals[3] = normals[1];
    normals[4] = glm::vec3(br_n._x, br_n._y, br_n._z ) / 127.0f;
    normals[5] = normals[2];
    /* bottom normals */
    normals[6] = normals[4];
    normals[7] = glm::vec3(bl_n._x, bl_n._y, bl_n._z ) / 127.0f;
    normals[8] = normals[2];
    /* left vertex (cw) */
    normals[9] = normals[7];
    normals[10] = normals[0];
    normals[11] = normals[2];

    return normals;
  }

  McnrChunk_s(int32_t memAbsOffset, void *in_buf) {
    int32_t buf_addr = reinterpret_cast<int32_t>(in_buf);
    void *dest_addr = reinterpret_cast<void*>(buf_addr + memAbsOffset);

    /* fill MCNR chunk with its real content */
    memcpy(this, dest_addr, sizeof(McnrChunk_s));
  }

 private:
  struct Normal_s { int8_t _x, _z, _y; } normal[145]; //<! normal (x, Z, y)
};

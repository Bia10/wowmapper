#include "adt_c.h"
#include <string.h>
#include <iostream>

#define MHDR_OFFSET 0xC
#define STEP 2.083333333f // 100.0f / (3.0f * 16.0f);

vertex3f::vertex3f() {}
vertex3f::vertex3f(float _x, float _y, float _z) : x(_x), y(_y), z(_z) { }

Adt_c::Adt_c(uint8_t **buffer)
    : raw_data_(*buffer),
      mhdr_chunk_(MHDR_OFFSET, raw_data_) {
  *buffer = NULL; // invalidate buffer
}

Adt_c::~Adt_c() {
  delete [] raw_data_;
}

void Adt_c::GenerateMesh() {
  glm::vec3 pos(0.0f, 0.0f, -32*STEP);
  for(int idx = 0; idx < 256; idx++) {
    McnkChunk_s &mcnk = *mhdr_chunk_.mcin->mcnk_index[idx].mcnk;
    //glm::vec3 pos(mcnk.position.y, -mcnk.position.z, mcnk.position.x);
    //pos += glm::vec3(-2666.67f, -49.3137, +11200.0f);
    if((idx % 16) == 0) {
      pos.z += 16*STEP;
      pos.x = 0;
    }

    pos.y = mcnk.position.z;
    pos.x += 16*STEP;

    /* fill vertex and normal array */
    for(int y = 0; y < 8; y++) {
      for(int x = 0; x < 8; x++) {
        const glm::vec3 *vertices = (*mcnk.mcvt)[y*8+x];
        const glm::vec3 *normals = (*mcnk.mcnr)[y*8+x];

        int32_t cur_idx = idx * 256 * 3 + (y * 8 + x) * 12;
        for(int off = 0; off < 12; off++) {
          vertices_[cur_idx+off] = vertices[off] + pos;
          normals_[cur_idx+off] = normals[off];
        }
      }
    }
  }
}

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


    for(int y = 0; y < 8; y++) {
      for(int x = 0; x < 8; x++) {
        /* get heigts from mcvt */
        float top_left = mcnk.mcvt->heights[y*17+x] + pos.y;
        float top_right = mcnk.mcvt->heights[y*17+x+1] + pos.y;
        float center = mcnk.mcvt->heights[y*17+x+9] + pos.y;
        float bottom_left = mcnk.mcvt->heights[y*17+x+17] + pos.y;
        float bottom_right = mcnk.mcvt->heights[y*17+x+18] + pos.y;

        /* center point of the patch */
        float patch_x = x*2*STEP +pos.x;
        float patch_y = y*2*STEP +pos.z;

        /* edge values to easily create points like top-left etc. */
        float edge_left = patch_x - STEP;
        float edge_right = patch_x + STEP;
        float edge_top = patch_y - STEP;
        float edge_bottom = patch_y + STEP;

        int32_t cur_idx = idx*256*3 + (y*8 + x) * 12;
        /* top vertex (cw) */
        map_patches_[cur_idx+0] = vertex3f(edge_left, top_left, edge_top);
        map_patches_[cur_idx+1] = vertex3f(edge_right, top_right, edge_top);
        map_patches_[cur_idx+2] = vertex3f(patch_x, center, patch_y);
        /* right vertex (cw) */
        map_patches_[cur_idx+3] = vertex3f(edge_right, top_right, edge_top);
        map_patches_[cur_idx+4] = vertex3f(edge_right, bottom_right, edge_bottom);
        map_patches_[cur_idx+5] = vertex3f(patch_x, center, patch_y);
        /* bottom vertex (cw) */
        map_patches_[cur_idx+6] = vertex3f(edge_right, bottom_right, edge_bottom);
        map_patches_[cur_idx+7] = vertex3f(edge_left, bottom_left, edge_bottom);
        map_patches_[cur_idx+8] = vertex3f(patch_x, center, patch_y);
        /* left vertex (cw) */
        map_patches_[cur_idx+9] = vertex3f(edge_left, bottom_left, edge_bottom);
        map_patches_[cur_idx+10] = vertex3f(edge_left, top_left, edge_top);
        map_patches_[cur_idx+11] = vertex3f(patch_x, center, patch_y);

        glm::vec3 norm = glm::cross(glm::vec3(edge_right, top_right, edge_top)-glm::vec3(edge_left, top_left, edge_top),
            glm::vec3(edge_right, top_right, edge_top)-glm::vec3(patch_x, center, patch_y));
        norm = glm::normalize(norm);
        map_normals_[cur_idx/3+0] = vertex3f(norm.x, norm.y, norm.z);

        norm = glm::cross(glm::vec3(edge_right, bottom_right, edge_bottom)-glm::vec3(edge_right, top_right, edge_top),
            glm::vec3(edge_right, bottom_right, edge_bottom)-glm::vec3(patch_x, center, patch_y));
        norm = glm::normalize(norm);
        map_normals_[cur_idx/3+1] = vertex3f(norm.x, norm.y, norm.z);

        norm = glm::cross(glm::vec3(edge_left, bottom_left, edge_bottom)-glm::vec3(edge_right, bottom_right, edge_bottom),
                    glm::vec3(edge_left, bottom_left, edge_bottom)-glm::vec3(patch_x, center, patch_y));
        norm = glm::normalize(norm);
        map_normals_[cur_idx/3+2] = vertex3f(norm.x, norm.y, norm.z);

        norm = glm::cross(glm::vec3(edge_left, top_left, edge_top)-glm::vec3(edge_left, bottom_left, edge_bottom),
                            glm::vec3(edge_left, top_left, edge_top)-glm::vec3(patch_x, center, patch_y));
        norm = glm::normalize(norm);
        map_normals_[cur_idx/3+3] = vertex3f(norm.x, norm.y, norm.z);


      }
    }
  }
}

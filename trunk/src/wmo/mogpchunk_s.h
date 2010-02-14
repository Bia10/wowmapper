#pragma once

#include "../chunk_s.h"

struct MogpChunk_s : Chunk_s {
  uint32_t group_name;        //!< offset into MOGN chunk
  uint32_t desc_group_name;   //!< offset into MOGN chunk
  uint32_t flags;
  glm::vec3 bbox_min;
  glm::vec3 bbox_max;
  uint16_t mopr_index;
  uint16_t mopr_num_items;
  uint16_t num_batches_a;
  uint16_t num_batches_b;
  uint32_t num_batches_c;
  uint8_t fog_indices[4];
  uint32_t _filler0;
  uint32_t wmo_group_id;
  uint32_t _filler1;
  uint32_t _filler2;

  MogpChunk_s(uint32_t offset, void *buffer) : Chunk_s(offset, buffer, false) {
    /* copy MOGP chunk manually to avoid memory corruption */
    uint32_t buf_addr = reinterpret_cast<uint32_t>(buffer);
    void *src_addr = reinterpret_cast<void*>(buf_addr + offset);
    memcpy(this, src_addr, sizeof(MogpChunk_s));
  }
};

#pragma once

#include "../chunk_s.h"

/*! @brief MODF chunk */
struct ModfChunk_s : Chunk_s {
  struct WmoInfo_s {
    uint32_t wmo_id;
    uint32_t uid;
    glm::vec3 position;
    glm::vec3 orientation;
    glm::vec3 bbox_min;
    glm::vec3 bbox_max;
    uint16_t _filler0;
    uint16_t doodad_set_index;
    uint32_t name_set;
  };

  typedef std::vector<WmoInfo_s> WmoInfo_t;
  WmoInfo_t wmo_info;

  ModfChunk_s(uint32_t offset, void *buffer) : Chunk_s(offset, buffer, false) {
    /* copy MODF chunk manually to avoid memory corruption */
    uint32_t buf_addr = reinterpret_cast<uint32_t>(buffer);
    void *src_addr = reinterpret_cast<void*>(buf_addr + offset + sizeof(Chunk_s));

    if(size == 0) return;

    wmo_info.resize(size / sizeof(WmoInfo_s));
    memcpy(&wmo_info[0], src_addr, size);
  }
};

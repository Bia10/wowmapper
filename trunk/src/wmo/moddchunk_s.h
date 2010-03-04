#pragma once

#include "../chunk_s.h"

/*! @brief MODD chunk. */
struct ModdChunk_s : Chunk_s {
  struct DoodadInfo_s {
    uint32_t name_offset;
    glm::vec3 position;
    glm::vec4 orientation;
    float scale;
    uint8_t color[4];
  };

  typedef std::vector<DoodadInfo_s> DoodadInfo_t;

  DoodadInfo_t doodad_info;

  ModdChunk_s() : Chunk_s() {

  }
};

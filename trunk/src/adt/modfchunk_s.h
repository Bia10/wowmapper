#pragma once

#include "../chunk_s.h"

/*! @brief MODF chunk */
struct ModfChunk_s : Chunk_s {
  struct WmoInfo_s {
    uint32_t id;
    uint32_t uid;
    glm::vec3 position;
    glm::vec3 orientation;
    glm::vec3 bbox_min;
    glm::vec3 bbox_max;
    uint16_t flags;
    uint16_t doodad_set_index;
    uint16_t name_set;
    uint16_t unknown;
  };

  typedef std::vector<WmoInfo_s> WmoInfo_t;
  WmoInfo_t wmo_info;

  ModfChunk_s() : Chunk_s() { }

  virtual void Initialize() {
    wmo_info.resize(size / sizeof(ModfChunk_s));
    CopyData(wmo_info.data());
  }
};

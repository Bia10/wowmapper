#pragma once

#include "../chunk_s.h"

struct MddfChunk_s : Chunk_s {
  struct DoodadInfo_s {
    uint32_t id;
    uint32_t uid;
    glm::vec3 position;
    glm::vec3 orientation;
    uint16_t scale;
    uint16_t flags;
  };

  typedef std::vector<DoodadInfo_s> DoodadInfos_t;

  DoodadInfos_t doodad_info;

  virtual void Initialize() {
    doodad_info.resize(size / sizeof(DoodadInfo_s));
    CopyData(doodad_info.data());
  }
};

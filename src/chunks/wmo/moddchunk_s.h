#pragma once

#include "../chunk_c.h"

struct ModdChunk_s : public Chunk_c {
  struct DoodadInfo_s {
    uint32_t name_offset;
    glm::vec3 position;
    glm::vec4 orientation;
    float scale;
    uint32_t lighting_color;
  };
  typedef std::vector<DoodadInfo_s> DoodadInfo_t;


  DoodadInfo_t doodad_info;

  ModdChunk_s(Chunk_c *parent) : Chunk_c(parent) { }

 protected:
  virtual void Initialize() {
    doodad_info.resize(buffer_.size()/sizeof(DoodadInfo_s));
    CopyDataBlock(buffer_, &doodad_info);
  }
};

#pragma once

#include "../chunk_c.h"

/*! \brief MDDF: Map Chunk Doodad Information. */
struct MddfChunk_s : public Chunk_c {
  struct DoodadInfo_s {
    uint32_t id;
    uint32_t uid;
    glm::vec3 position;
    glm::vec3 orientation;
    uint16_t scale;
    uint16_t flags;
  };
  typedef std::vector<DoodadInfo_s> DoodadInfo_t;

  DoodadInfo_t doodad_info;

  MddfChunk_s(Chunk_c *parent) : Chunk_c(parent) {}

 protected:
  virtual void LateInit() {
    doodad_info.resize(buffer_.size()/sizeof(DoodadInfo_s));
    CopyDataBlock(buffer_, &doodad_info);
  }
};

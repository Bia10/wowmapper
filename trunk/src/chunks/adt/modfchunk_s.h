#pragma once

#include "../chunk_c.h"

/*! \brief MODF chunk.
 *  http://www.madx.dk/wowdev/wiki/index.php?title=ADT/v18#MODF_chunk */
struct ModfChunk_s : public Chunk_c {
  struct WmoInfo_s {
    uint32_t id;
    uint32_t uid;
    glm::vec3 position;
    glm::vec3 orientation;
    glm::vec3 bbox_min;
    glm::vec3 bbox_max;
    uint32_t flags;
    uint16_t doodad_set;
    uint16_t name_set;
  };
  typedef std::vector<WmoInfo_s> WmoInfo_t;

  WmoInfo_t wmo_info;

  ModfChunk_s(Chunk_c *parent) : Chunk_c(parent) {}

 protected:
  virtual void Initialize() {
    wmo_info.resize(buffer_.size()/sizeof(WmoInfo_s));
    CopyDataBlock(buffer_, &wmo_info);
  }
};

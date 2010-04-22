#pragma once

#include "../chunk_c.h"

/*! \brief MODF: Map Chunk Wmo Information. */
struct ModfChunk_s : public Chunk_c {
  struct WmoInfo_s;
  typedef std::vector<WmoInfo_s> WmoInfos_t;

  WmoInfos_t wmo_infos;

  ModfChunk_s(Chunk_c *parent, off_t off);
};

struct ModfChunk_s::WmoInfo_s {
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

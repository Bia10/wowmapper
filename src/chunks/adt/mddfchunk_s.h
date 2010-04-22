#pragma once

#include "../chunk_c.h"

/*! \brief MDDF: Map Chunk Doodad Information. */
struct MddfChunk_s : public Chunk_c {
  struct DoodadInfo_s;
  typedef std::vector<DoodadInfo_s> DoodadInfos_t;

  DoodadInfos_t doodad_infos;

  MddfChunk_s(Chunk_c *parent, off_t off);
};


struct MddfChunk_s::DoodadInfo_s {
  uint32_t id;
  uint32_t uid;
  glm::vec3 position;
  glm::vec3 orientation;
  uint16_t scale;
  uint16_t flags;
};

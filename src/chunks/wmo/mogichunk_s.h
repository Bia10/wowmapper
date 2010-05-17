#pragma once

#include "../chunk_c.h"

/*! \brief MOGI: Map Object Group Information. */
struct MogiChunk_s : public Chunk_c {
  struct GroupInfo_s {
    uint32_t flags;
    glm::vec3 bb_min, bb_max;
    wm_off_t name_off;
  };
  typedef std::vector<GroupInfo_s> GroupInfos_t;

  GroupInfos_t group_infos;


  MogiChunk_s(Chunk_c *parent, wm_off_t off)
      : Chunk_c(parent, off) {
    wm_size_t num_infos = GetSize() / sizeof(GroupInfo_s);
    group_infos.resize(num_infos);
    CopyVector(GetBuffer(), GetCurOffset()+DATA_OFFSET, num_infos, &group_infos);
  }
};

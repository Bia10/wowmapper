#pragma once

#include "../chunk_c.h"

/*! \brief MOGI: Map Object Group Information. */
struct MogiChunk_s : public Chunk_c {
  struct GroupInfo_s {
    uint32_t flags;
    glm::vec3 bb_min, bb_max;
    uint32_t name_index;
  };
  typedef std::vector<GroupInfo_s> GroupInfo_t;


  GroupInfo_t group_info;


  MogiChunk_s(Chunk_c *parent) : Chunk_c(parent) { }

 protected:
  virtual void LateInit() {
    group_info.resize(buffer_.size()/sizeof(GroupInfo_s));
    CopyDataBlock(buffer_, &group_info);
  }
};

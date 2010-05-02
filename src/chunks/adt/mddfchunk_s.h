#pragma once

#include "../chunk_c.h"

/*! \brief MDDF: Map Chunk Doodad Information. */
struct MddfChunk_s : public Chunk_c {
  struct DoodadInfo_s {
    uint32_t id;
    uint32_t uid;
    Vec3_t pos;
    Vec3_t rot;
    uint16_t scale;
    uint16_t flags;
  };
  typedef std::vector<DoodadInfo_s> DoodadInfos_t;

  DoodadInfos_t doodad_infos;

  MddfChunk_s(Chunk_c *parent, off_t off)
      : Chunk_c(parent, off) {
    size_t num_doodads = GetValue<size_t>(0x0, SIZE_OFFSET) / sizeof(DoodadInfo_s);
    doodad_infos.resize(num_doodads);
    CopyVector(GetBuffer(), GetCurOffset()+DATA_OFFSET, num_doodads, &doodad_infos);
  }
};

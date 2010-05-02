#pragma once

#include "../chunk_c.h"

/*! \brief MODF: Map Chunk Wmo Information. */
struct ModfChunk_s : public Chunk_c {
  struct WmoInfo_s {
    uint32_t id;
    uint32_t uid;
    Vec3_t pos;
    Vec3_t rot;
    BBox_s bbox;
    uint32_t flags;
    uint16_t doodad_set;
    uint16_t name_set;
  };
  typedef std::vector<WmoInfo_s> WmoInfos_t;

  WmoInfos_t wmo_infos;

  ModfChunk_s(Chunk_c *parent, off_t off)
      : Chunk_c(parent, off) {
    size_t num_infos = GetSize() / sizeof(WmoInfo_s);
    wmo_infos.resize(num_infos);
    CopyVector(GetBuffer(), GetCurOffset()+DATA_OFFSET, num_infos, &wmo_infos);
  }
};

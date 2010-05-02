#pragma once

#include "../chunk_c.h"

/*! \brief MOPY: Map Object Material Info. */
struct MopyChunk_s : public Chunk_c {
  struct MaterialInfo_s {
    uint8_t flags;
    uint8_t id;
  };
  typedef std::vector<MaterialInfo_s> MaterialInfos_t;

  MaterialInfos_t mat_infos;


  MopyChunk_s(Chunk_c *parent, off_t off)
      : Chunk_c(parent, off) {
    size_t num_infos = GetSize() / sizeof(MaterialInfo_s);
    mat_infos.resize(num_infos);
    CopyVector(GetBuffer(), GetCurOffset()+DATA_OFFSET, num_infos, &mat_infos);
  }
};

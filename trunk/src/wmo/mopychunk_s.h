#pragma once

#include "../chunk_s.h"

/*! \brief MOPY chunk. */
struct MopyChunk_s : Chunk_s {
  struct PolyInfo_s {
    uint8_t flags;
    uint8_t material_id;
  };

  typedef std::vector<PolyInfo_s> PolyInfo_t;

  PolyInfo_t poly_info;

  virtual void Initialize() {
    poly_info.resize(size / sizeof(PolyInfo_s));
    CopyData(poly_info.data());
  }
};

#pragma once

#include "../chunk_c.h"

/*! \brief MOGN: Map Object Group Names. */
struct MognChunk_s : public Chunk_c {
  std::string group_names;


  MognChunk_s(Chunk_c *parent, wm_off_t off)
      : Chunk_c(parent, off) {
    wm_size_t name_size = GetSize();
    Buffer_t names(name_size);
    CopyVector(GetBuffer(), GetCurOffset()+DATA_OFFSET, name_size, &names);
    group_names.assign(names.begin(), names.end());
  }
};

#pragma once

#include "../chunk_c.h"

struct ModnChunk_s : public Chunk_c {
  std::string doodad_names;

  ModnChunk_s(Chunk_c *parent, off_t off)
      : Chunk_c(parent, off) {
    size_t name_size = GetSize();
    Buffer_t names(name_size);
    CopyVector(GetBuffer(), GetCurOffset()+DATA_OFFSET, name_size, &names);
    doodad_names.assign(names.begin(), names.end());
  }
};

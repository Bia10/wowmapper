#pragma once

#include "../chunk_c.h"

/*! \brief MODS: Map Object Doodad Sets. */
struct ModsChunk_s : public Chunk_c {
  struct DoodadSet_s {
    char set_name[20];
    off_t start_index;
    size_t num_doodads;
   private:
    uint32_t filler_0;
  };
  typedef std::vector<DoodadSet_s> DoodadSets_t;

  DoodadSets_t doodad_sets;

  ModsChunk_s(Chunk_c *parent, off_t off)
      : Chunk_c(parent, off) {
    size_t num_sets = GetSize() / sizeof(DoodadSet_s);
    doodad_sets.resize(num_sets);
    CopyVector(GetBuffer(), GetCurOffset()+DATA_OFFSET, num_sets, &doodad_sets);
  }
};

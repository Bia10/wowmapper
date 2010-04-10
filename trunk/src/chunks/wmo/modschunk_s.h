#pragma once

#include "../chunk_c.h"

struct ModsChunk_s : public Chunk_c {
  struct DoodadSets_s {
    char set_name[20];
    uint32_t start_index;
    uint32_t num_doodads;
   private:
    uint32_t filler_0;
  };
  typedef std::vector<DoodadSets_s> DoodadSets_t;


  DoodadSets_t doodad_sets;

  ModsChunk_s(Chunk_c *parent) : Chunk_c(parent) { }

 protected:
  virtual void LateInit() {
    doodad_sets.resize(buffer_.size()/sizeof(DoodadSets_s));
    CopyDataBlock(buffer_, &doodad_sets);
  }
};

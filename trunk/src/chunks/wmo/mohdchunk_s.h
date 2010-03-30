#pragma once

#include "../chunk_c.h"

struct MohdChunk_s : public Chunk_c {
  uint32_t num_groups;
  uint32_t num_models;
  uint32_t num_doodads;
  uint32_t num_doodad_sets;
  uint32_t wmo_id;
  uint32_t liquid_type;

  MohdChunk_s(Chunk_c *parent) : Chunk_c(parent) { }

 protected:
  virtual void Initialize() {
    num_groups = GetField<uint32_t>(0x04);
    num_models =  GetField<uint32_t>(0x10);
    num_doodads = GetField<uint32_t>(0x14);
    num_doodad_sets = GetField<uint32_t>(0x18);
    wmo_id = GetField<uint32_t>(0x20);
    liquid_type = GetField<uint32_t>(0x3c);
  }
};

#pragma once

#include "../chunk_c.h"

/*! \brief MOHD: Map Object Header. */
struct MohdChunk_s : public Chunk_c {
  size_t num_groups;
  size_t num_models;
  size_t num_doodads;
  size_t num_doodad_sets;
  uint32_t wmo_id;
  uint32_t liquid_type;


  MohdChunk_s(Chunk_c *parent, off_t off)
      : Chunk_c(parent, off),
        num_groups(GetValue<size_t>(0x04)),
        num_models(GetValue<size_t>(0x10)),
        num_doodads(GetValue<size_t>(0x14)),
        num_doodad_sets(GetValue<size_t>(0x18)),
        wmo_id(GetValue<size_t>(0x20)),
        liquid_type(GetValue<size_t>(0x3c)) {

  }
};

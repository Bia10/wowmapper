#pragma once

#include "../chunk_c.h"

/*! \brief MOHD: Map Object Header. */
struct MohdChunk_s : public Chunk_c {
  wm_size_t num_groups;
  wm_size_t num_models;
  wm_size_t num_doodads;
  wm_size_t num_doodad_sets;
  uint32_t wmo_id;
  uint32_t liquid_type;


  MohdChunk_s(Chunk_c *parent, wm_off_t off)
      : Chunk_c(parent, off),
        num_groups(GetValue<wm_size_t>(0x04)),
        num_models(GetValue<wm_size_t>(0x10)),
        num_doodads(GetValue<wm_size_t>(0x14)),
        num_doodad_sets(GetValue<wm_size_t>(0x18)),
        wmo_id(GetValue<wm_size_t>(0x20)),
        liquid_type(GetValue<wm_size_t>(0x3c)) {

  }
};

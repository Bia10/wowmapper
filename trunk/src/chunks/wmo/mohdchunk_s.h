/*  wowmapper - World of Warcraft MAP PARser
    Copyright (C) 2010 PferdOne

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>. */

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

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

/*! \brief MODS: Map Object Doodad Sets. */
struct ModsChunk_s : public Chunk_c {
  struct DoodadSet_s {
    char set_name[20];
    wm_off_t start_index;
    wm_size_t num_doodads;
   private:
    uint32_t filler_0;
  };
  typedef std::vector<DoodadSet_s> DoodadSets_t;

  DoodadSets_t doodad_sets;

  ModsChunk_s(Chunk_c *parent, wm_off_t off)
      : Chunk_c(parent, off) {
    wm_size_t num_sets = GetSize() / sizeof(DoodadSet_s);
    doodad_sets.resize(num_sets);
    CopyVector(GetBuffer(), GetCurOffset()+DATA_OFFSET, num_sets, &doodad_sets);
  }
};

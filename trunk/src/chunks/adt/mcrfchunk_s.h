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

/*! \brief MCRF: Map Chunk references. */
struct McrfChunk_s : public Chunk_c {
  Indices32_t doodad_offs;
  Indices32_t wmo_offs;


  McrfChunk_s(Chunk_c *parent, wm_off_t off, wm_size_t num_doodads, wm_size_t num_wmos)
      : Chunk_c(parent, off) {
    wm_off_t ref_off = GetCurOffset() + DATA_OFFSET;
    wm_off_t wmo_off = ref_off+num_doodads*sizeof(uint32_t);
    
    doodad_offs.resize(num_doodads);
    CopyVector(GetBuffer(), ref_off, num_doodads, &doodad_offs);

    wmo_offs.resize(num_wmos);
    CopyVector(GetBuffer(), wmo_off, num_wmos, &wmo_offs);
  }
};

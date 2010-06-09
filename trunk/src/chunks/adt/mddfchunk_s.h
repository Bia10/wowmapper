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

/*! \brief MDDF: Map Chunk Doodad Information. */
struct MddfChunk_s : public Chunk_c {
  struct DoodadInfo_s {
    uint32_t id;
    uint32_t uid;
    Vec3_t pos;
    Vec3_t rot;
    uint16_t scale;
    uint16_t flags;
  };
  typedef std::vector<DoodadInfo_s> DoodadInfos_t;

  DoodadInfos_t doodad_infos;

  MddfChunk_s(Chunk_c *parent, wm_off_t off)
      : Chunk_c(parent, off) {
    wm_size_t num_doodads = GetSize() / sizeof(DoodadInfo_s);
    doodad_infos.resize(num_doodads);
    CopyVector(GetBuffer(), GetCurOffset()+DATA_OFFSET, num_doodads, &doodad_infos);
  }
};

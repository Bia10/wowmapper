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

/*! \brief MODF: Map Chunk Wmo Information. */
struct ModfChunk_s : public Chunk_c {
  struct WmoInfo_s {
    uint32_t id;
    uint32_t uid;
    Vec3_t pos;
    Vec3_t rot;
    BBox_s bbox;
    uint32_t flags;
    uint16_t doodad_set;
    uint16_t name_set;
  };
  typedef std::vector<WmoInfo_s> WmoInfos_t;

  WmoInfos_t wmo_infos;

  ModfChunk_s(Chunk_c *parent, wm_off_t off)
      : Chunk_c(parent, off) {
    wm_size_t num_infos = GetSize() / sizeof(WmoInfo_s);
    wmo_infos.resize(num_infos);
    CopyVector(GetBuffer(), GetCurOffset()+DATA_OFFSET, num_infos, &wmo_infos);
  }
};

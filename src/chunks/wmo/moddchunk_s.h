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

/*! \brief MODD: Map Object Doodad Information. */
struct ModdChunk_s : public Chunk_c {
  struct DoodadInfo_s {
    wm_off_t name_off;           //!< Offset to name
    glm::vec3 pos;            //!< Position
    glm::quat rot;            //!< Rotation (quaternion)
    float scale;              //!< Scale
    uint32_t lighting_color;  //!< Lighting Color
  };
  typedef std::vector<DoodadInfo_s> DoodadInfos_t;

  DoodadInfos_t doodad_infos;


  ModdChunk_s(Chunk_c *parent, wm_off_t off)
      : Chunk_c(parent, off) {
    wm_size_t num_infos = GetSize() / sizeof(DoodadInfo_s);
    doodad_infos.resize(num_infos);

    CopyVector(GetBuffer(), GetCurOffset()+DATA_OFFSET, num_infos, &doodad_infos);
  }
};

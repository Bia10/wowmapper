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

/*! \brief MONR: Map Object Normals. */
struct MonrChunk_s : public Chunk_c {
  Normals_t normals;

  MonrChunk_s(Chunk_c *parent, wm_off_t off)
      : Chunk_c(parent, off) {
    wm_size_t num_normals = GetSize() / sizeof(glm::vec3);
    normals.resize(num_normals);
    CopyVector(GetBuffer(), GetCurOffset()+DATA_OFFSET, num_normals, &normals);
  }
};

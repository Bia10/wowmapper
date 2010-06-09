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

/*! \brief MCVT: Map Chunk Vertices. */
struct McvtChunk_s: public Chunk_c {
  std::vector<float> heightmap;

  McvtChunk_s(Chunk_c *parent, wm_off_t off)
      : Chunk_c(parent, off), heightmap(145) {
    CopyVector(GetBuffer(), GetCurOffset() + DATA_OFFSET, 145, &heightmap);
  }

  void GetVertices(Vertices_t *vertices) const {
    vertices->resize(145);

    for (int i = 0; i < 145; i++) {
      uint32_t row_idx = i / 17;
      uint32_t col_idx = i % 17;

      bool isRow9x9 = col_idx < 9;
      if (isRow9x9) { // if it's a row of the outer 9x9 vector ...
        (*vertices)[i].x = col_idx * TU * 2;
        (*vertices)[i].z = row_idx * TU * 2;
      } else { // if it's a row of the inner 8x8 vector ...
        (*vertices)[i].x = (col_idx - 9) * TU * 2 + TU;
        (*vertices)[i].z = row_idx * TU * 2 + TU;
      }
      (*vertices)[i].y = heightmap[i];
    }
  }

  void GetIndices(Indices32_t *indices) const {
    indices->resize(768);

    for (int y = 0; y < 8; y++) {
      for (int x = 0; x < 8; x++) {
        uint32_t i = y * 8 * 12 + x * 12;

        // vertex 0
        (*indices)[i + 0] = y * 17 + x; // top left
        (*indices)[i + 2] = y * 17 + x + 1; // top right
        (*indices)[i + 1] = y * 17 + x + 9; // center

        // vertex 1
        (*indices)[i + 3] = y * 17 + x + 1; // top right
        (*indices)[i + 5] = y * 17 + x + 18; // bottom right
        (*indices)[i + 4] = y * 17 + x + 9; // center

        // vertex 2
        (*indices)[i + 6] = y * 17 + x + 18; // bottom right
        (*indices)[i + 8] = y * 17 + x + 17; // bottom left
        (*indices)[i + 7] = y * 17 + x + 9; // center

        // vertex 3
        (*indices)[i + 9] = y * 17 + x + 17; // bottom left
        (*indices)[i + 11] = y * 17 + x; // top left
        (*indices)[i + 10] = y * 17 + x + 9; // center
      }
    }
  }
};

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

static float NormalsI8toF(int8_t a) {
  return a * 0.007874016f; // a * (1/127)
}

/*! \brief MCNR: Map Chunk Normals. */
struct McnrChunk_s : public Chunk_c {
  std::vector<float> normals;


  McnrChunk_s(Chunk_c *parent, wm_off_t off)
      : Chunk_c(parent, off), normals(435) {
    // temporary normals array to hold these stupid 8 bit normals by blizz
    std::vector<int8_t> temp_normals(435);
    CopyVector(GetBuffer(), GetCurOffset()+DATA_OFFSET, 435, &temp_normals);
    // transform to float normals
    std::transform(temp_normals.begin(), temp_normals.end(),
        normals.begin(), NormalsI8toF);
  }

  void GetNormals(Vertices_t *norm) const {
    norm->reserve(145);
    for (int i = 0; i < 145; i++) {
      norm->push_back(glm::vec3(normals[i*3+0],    // x
                                normals[i*3+1],    // z
                                normals[i*3+2]));  // y
    }
  }
};






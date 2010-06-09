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

#include "mcvtchunk_s.h"
#include "mcnrchunk_s.h"
#include "mcrfchunk_s.h"

/*! \brief MCNK: Map Chunks.
 *  \remark Provides terrain indices, vertices and normals. Also*/
struct McnkChunk_s : public Chunk_c {
  uint32_t flags;
  uint32_t index_x;
  uint32_t index_y;
  wm_size_t num_layers;
  wm_size_t num_doodads;
  wm_off_t mcvt_off;
  wm_off_t mcnr_off;
  wm_off_t mcly_off;
  wm_off_t mcrf_off;
  wm_off_t mcal_off;
  wm_size_t mcal_size;
  wm_off_t mcsh_off;
  wm_size_t mcsh_size;
  uint32_t area_id;
  wm_size_t num_wmos;
  uint32_t holes;
  uint64_t texmap_0;
  uint64_t texmap_1;
  wm_off_t mclq_off;
  wm_size_t mclq_size;
  glm::vec3 position;
  wm_off_t mccv_off;

  McvtChunk_s mcvt;
  McnrChunk_s mcnr;
  McrfChunk_s mcrf;


  McnkChunk_s(Chunk_c *parent, wm_off_t off)
      : Chunk_c(parent, off),
        flags(GetValue<uint32_t>(0x00)),
        index_x(GetValue<uint32_t>(0x04)),
        index_y(GetValue<uint32_t>(0x08)),
        num_doodads(GetValue<wm_size_t>(0x10)),
        mcvt_off(GetValue<wm_off_t>(0x14)),
        mcnr_off(GetValue<wm_off_t>(0x18)),
        mcly_off(GetValue<wm_off_t>(0x1c)),
        mcrf_off(GetValue<wm_off_t>(0x20)),
        mcal_off(GetValue<wm_off_t>(0x24)),
        mcal_size(GetValue<wm_size_t>(0x28)),
        mcsh_off(GetValue<wm_off_t>(0x2c)),
        mcsh_size(GetValue<wm_size_t>(0x30)),
        area_id(GetValue<uint32_t>(0x34)),
        num_wmos(GetValue<wm_size_t>(0x38)),
        holes(GetValue<uint32_t>(0x3c)),
        texmap_0(GetValue<uint64_t>(0x40)),
        texmap_1(GetValue<uint64_t>(0x48)),
        mclq_off(GetValue<wm_off_t>(0x60)),
        mclq_size(GetValue<wm_size_t>(0x64)),
        position(GetValue<Vec3_t>(0x68)),
        mccv_off(GetValue<wm_off_t>(0x74)),

        mcvt(this, mcvt_off),
        mcnr(this, mcnr_off),
        mcrf(this, mcrf_off, num_doodads, num_wmos) {

  }
};

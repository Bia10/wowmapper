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

/*! \brief MCIN: Map Chunk Index. Contains 256 entries for offests to every
 *         map chunk in this ADT. */
struct McinChunk_s : public Chunk_c {
  struct McnkInfo_s {
    wm_off_t mcnk_off;         //!< absolute offset
    wm_size_t size;            //!< the size of the MCNK chunk, this is refering to
    uint32_t unused_flags;  // these two are always 0, only set in the client
    uint32_t unused_async;
  };
  typedef std::vector<McnkInfo_s> McnkInfos_t;

  McnkInfos_t mcnk_info;     //!< contains 256 (16*16) MCNK info entries


  McinChunk_s(Chunk_c *parent, wm_off_t off)
      : Chunk_c(parent, off), mcnk_info(256) {
    CopyVector(GetBuffer(), GetCurOffset()+DATA_OFFSET, 256, &mcnk_info);
  }
};



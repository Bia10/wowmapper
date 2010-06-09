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

/*! \brief MAIN chunk. */
struct MainChunk_s : public Chunk_c {
  struct MapTile_s {
    uint32_t flags;
    uint32_t async_object;
  };
  typedef std::vector<MapTile_s> MapTiles_t;

  MapTiles_t map_tiles;


  MainChunk_s(Chunk_c *parent, wm_off_t off)
      : Chunk_c(parent, off), map_tiles(4096) {
    CopyVector(GetBuffer(), GetCurOffset()+DATA_OFFSET, 4096, &map_tiles);
  }
};

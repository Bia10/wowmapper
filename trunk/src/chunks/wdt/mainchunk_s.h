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

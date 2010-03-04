#pragma once

#include "../chunk_s.h"

/*! \brief MAIN chunk. */
struct MainChunk_s : Chunk_s {
  struct MapTile_s {
    int32_t flags;
    void *async_object;
  };

  typedef std::vector<MapTile_s> MapTiles_t;

  MapTiles_t map_tiles;


  MainChunk_s() : Chunk_s(), map_tiles(4096) { }

  virtual void Initialize() {
    CopyData(map_tiles.data());
  }
};

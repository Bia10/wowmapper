#pragma once

#include "../chunk_s.h"
#include <stdio.h>

/*! @brief MAIN chunk. */
struct MainChunk_s : Chunk_s {
  struct MapTile_s {
    int32_t flags;
    void *async_object;
  };

  MapTile_s map_tile[64][64]; // y * x

  MainChunk_s(uint32_t offset, void *buffer) : Chunk_s(offset, buffer, true) {}
};

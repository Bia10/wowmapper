#pragma once

#include "../chunk_c.h"

/*! \brief MH2O: Map Chunk H2O (water). */
struct Mh2oChunk_s : public Chunk_c {
  struct Mh2oHeader_s;
  struct Mh2oInfo_s;
  struct Mh2oContent_s;

  typedef std::vector<Mh2oHeader_s> Mh2oHeaders_t;
  typedef std::vector<Mh2oInfo_s> Mh2oInfos_t;
  typedef std::vector<Mh2oContent_s> Mh2oContent_t;
  typedef std::vector<std::vector<float> > Heightmaps_t;

  Mh2oHeaders_t header;
  Mh2oContent_t content;

  Mh2oChunk_s(Chunk_c *parent, off_t off);
};


struct Mh2oChunk_s::Mh2oHeader_s {
  off_t info_off;
  size_t num_layers;
  off_t mask_off;
};

struct Mh2oChunk_s::Mh2oInfo_s {
  uint16_t type;
  uint16_t flags;
  float height_level_1;
  float height_level_2;
  uint8_t x_offset;
  uint8_t y_offset;
  uint8_t width;
  uint8_t height;
  off_t mask2_off;
  off_t heightmap_off;
};

struct Mh2oChunk_s::Mh2oContent_s {
  Indices64_t masks;
  Mh2oInfos_t infos;
  Heightmaps_t heightmaps;
  size_t num_layers;
};

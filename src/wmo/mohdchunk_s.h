#pragma once

#include "../chunk_s.h"

struct MohdChunk_s : Chunk_s {
  uint32_t num_textures;  //<! number of textures
  uint32_t num_groups;    //<! number of WMO groups
  uint32_t num_portals;   //<! number of portals
  uint32_t num_lights;    //<! number of lights
  uint32_t num_models;    //<! number of M2 models imported
  uint32_t num_doodads;   //<! number of doodads
  uint32_t num_sets;      //<! number of doodad sets
  uint32_t ambient_color; //<! ???
  uint32_t wmo_id;        //<! WMO ID (column 2 in WMOAreaTable.dbc)
  glm::vec3 bbox_min;     //<! Bounding Box min
  glm::vec3 bbox_max;     //<! Bounding Box max
  uint32_t _filler0;      //<! Unknown

  MohdChunk_s(uint32_t offset, void *buffer) : Chunk_s(offset, buffer, true) { }
};

#pragma once

#include "../chunk_c.h"
#include "mcvtchunk_s.h"
#include "mcnrchunk_s.h"
#include "mcrfchunk_s.h"

/*! \brief MCNK: Map Chunks.
 *  \remark Provides terrain indices, vertices and normals. Also*/
struct McnkChunk_s : public Chunk_c {
  uint32_t flags;
  uint32_t index_x;
  uint32_t index_y;
  size_t num_layers;
  size_t num_doodads;
  off_t mcvt_off;
  off_t mcnr_off;
  off_t mcly_off;
  off_t mcrf_off;
  off_t mcal_off;
  size_t mcal_size;
  off_t mcsh_off;
  size_t mcsh_size;
  uint32_t area_id;
  size_t num_wmos;
  uint32_t holes;
  uint64_t texmap_0;
  uint64_t texmap_1;
  off_t mclq_off;
  size_t mclq_size;
  glm::vec3 position;
  off_t mccv_off;

  McvtChunk_s mcvt;
  McnrChunk_s mcnr;
  McrfChunk_s mcrf;

  McnkChunk_s(Chunk_c *parent, off_t off);
};

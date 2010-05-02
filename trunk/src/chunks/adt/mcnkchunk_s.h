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


  McnkChunk_s(Chunk_c *parent, off_t off)
      : Chunk_c(parent, off),
        flags(GetValue<uint32_t>(0x00)),
        index_x(GetValue<uint32_t>(0x04)),
        index_y(GetValue<uint32_t>(0x08)),
        num_doodads(GetValue<size_t>(0x10)),
        mcvt_off(GetValue<off_t>(0x14)),
        mcnr_off(GetValue<off_t>(0x18)),
        mcly_off(GetValue<off_t>(0x1c)),
        mcrf_off(GetValue<off_t>(0x20)),
        mcal_off(GetValue<off_t>(0x24)),
        mcal_size(GetValue<size_t>(0x28)),
        mcsh_off(GetValue<off_t>(0x2c)),
        mcsh_size(GetValue<size_t>(0x30)),
        area_id(GetValue<uint32_t>(0x34)),
        num_wmos(GetValue<size_t>(0x38)),
        holes(GetValue<uint32_t>(0x3c)),
        texmap_0(GetValue<uint64_t>(0x40)),
        texmap_1(GetValue<uint64_t>(0x48)),
        mclq_off(GetValue<off_t>(0x60)),
        mclq_size(GetValue<size_t>(0x64)),
        position(GetValue<Vec3_t>(0x68)),
        mccv_off(GetValue<off_t>(0x74)),

        mcvt(this, mcvt_off),
        mcnr(this, mcnr_off),
        mcrf(this, mcrf_off, num_doodads, num_wmos) {

  }
};

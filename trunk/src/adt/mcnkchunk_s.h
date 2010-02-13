#pragma once

#include <glm/glm.hpp>
#include <iostream>
#include <exception>
#include "mcvtchunk_s.h"
#include "mcnrchunk_s.h"

/*! @brief MCNK chunk */
struct McnkChunk_s : Chunk_s {
  uint32_t flags;
  uint32_t index_x;
  uint32_t index_y;
  uint32_t num_layers;
  uint32_t num_doodad_refs;
  McvtChunk_s* mcvt;        //!< heights
  McnrChunk_s* mcnr;        //!< normals
  void* offset_layer;
  void* offset_refs;
  void* offset_alpha;
  uint32_t size_alpha;
  void* offset_shadow;
  uint32_t size_shadow;
  uint32_t area_id;
  uint32_t num_map_obj_refs;
  uint32_t holes;
  uint32_t really_low_quality_texturing_map[2][2];  //!< original uint2[8][8]
  uint32_t pred_tex;
  uint32_t no_effect_doodad;
  void* offset_snd_emitters;
  uint32_t num_snd_emitters;
  void* offset_liquid;
  uint32_t size_liquid;
  glm::vec3 position;
  void* offset_mccv;
  uint32_t unused_pad1;
  uint32_t unused_pad2;

  enum {
    FLAG_MCSH,
    FLAG_IMPASS,
    FLAG_LQ_RIVER,
    FLAG_LQ_OCEAN,
    FLAG_LQ_MAGMA,
    FLAG_MCCV,
  };

  McnkChunk_s(uint32_t offset, void *buffer) : Chunk_s(offset, buffer, false) {
    /* copy MCNK chunk manually to avoid memory corruption */
    uint32_t buf_addr = reinterpret_cast<uint32_t>(buffer);
    void *src_addr = reinterpret_cast<void*>(buf_addr + offset);
    memcpy(this, src_addr, sizeof(McnkChunk_s));

    /* intialize sub chunks: MCVT */
    uint32_t mcvt_offset = reinterpret_cast<uint32_t>(mcvt);
    mcvt = new McvtChunk_s(offset + mcvt_offset, buffer);
    /* intialize sub chunks: MCNR */
    uint32_t mcnr_offset = reinterpret_cast<uint32_t>(mcnr);
    mcnr = new McnrChunk_s(offset + mcnr_offset, buffer);
  }

  ~McnkChunk_s() {
    SAFE_DELETE(mcvt);
    SAFE_DELETE(mcnr);
  }
};

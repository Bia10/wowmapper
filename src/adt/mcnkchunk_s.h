#pragma once

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
  float position[3];
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

  McnkChunk_s(int32_t memAbsOffset, void *in_buf) {
    int32_t buf_addr = reinterpret_cast<int32_t>(in_buf);
    void *dest_addr = reinterpret_cast<void*>(buf_addr + memAbsOffset);

    /* fill MCVT chunk with its real content */
    memcpy(this, dest_addr, sizeof(McnkChunk_s));

    /* intialize sub chunks: MCVT */
    int32_t mcvt_offset = reinterpret_cast<int32_t>(mcvt);
    mcvt = new McvtChunk_s(memAbsOffset + mcvt_offset, in_buf);
    /* intialize sub chunks: MCNR */
    int32_t mcnr_offset = reinterpret_cast<int32_t>(mcnr);
    mcnr = new McnrChunk_s(memAbsOffset + mcnr_offset, in_buf);
  }

  ~McnkChunk_s() {
    SAFE_DELETE(mcvt);
    SAFE_DELETE(mcnr);
  }
};

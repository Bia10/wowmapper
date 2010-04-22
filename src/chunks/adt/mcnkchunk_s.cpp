#include "mcnkchunk_s.h"

McnkChunk_s::McnkChunk_s(Chunk_c *parent, off_t off)
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
      position(GetValue<glm::vec3>(0x68)),
      mccv_off(GetValue<off_t>(0x74)),

      mcvt(this, mcvt_off),
      mcnr(this, mcnr_off),
      mcrf(this, mcrf_off) {

}

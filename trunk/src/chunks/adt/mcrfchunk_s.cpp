#include "mcrfchunk_s.h"
#include "mcnkchunk_s.h"

McrfChunk_s::McrfChunk_s(Chunk_c *parent, off_t off)
    : Chunk_c(parent, off) {
  off_t ref_off = GetCurOffset() + DATA_OFFSET;
  size_t num_doodads = reinterpret_cast<McnkChunk_s*>(parent_)->num_doodads;
  size_t num_wmos = reinterpret_cast<McnkChunk_s*>(parent_)->num_wmos;
  off_t wmo_off = ref_off+num_doodads*sizeof(uint32_t);

  doodad_offs.resize(num_doodads);
  wmo_offs.resize(num_wmos);

  CopyVector(GetBuffer(), ref_off, num_doodads, &doodad_offs);
  CopyVector(GetBuffer(), wmo_off, num_wmos, &wmo_offs);
}

#include "mddfchunk_s.h"

MddfChunk_s::MddfChunk_s(Chunk_c *parent, off_t off)
    : Chunk_c(parent, off) {
  size_t num_doodads = GetValue<size_t>(-0x4) / sizeof(DoodadInfo_s);
  doodad_infos.resize(num_doodads);
  CopyVector(GetBuffer(), GetCurOffset()+DATA_OFFSET, num_doodads, &doodad_infos);
}

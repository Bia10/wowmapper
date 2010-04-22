#include "mhdrchunk_s.h"

MhdrChunk_s::MhdrChunk_s(Chunk_c *parent, off_t off)
    : Chunk_c(parent, off) {
  mcin_off = GetValue<off_t>(0x04) + SUB_OFFSET;
  mtex_off = GetValue<off_t>(0x08) + SUB_OFFSET;
  mmdx_off = GetValue<off_t>(0x0c) + SUB_OFFSET;
  mmid_off = GetValue<off_t>(0x10) + SUB_OFFSET;
  mwmo_off = GetValue<off_t>(0x14) + SUB_OFFSET;
  mwid_off = GetValue<off_t>(0x18) + SUB_OFFSET;
  mddf_off = GetValue<off_t>(0x1c) + SUB_OFFSET;
  modf_off = GetValue<off_t>(0x20) + SUB_OFFSET;
  mfbo_off = GetValue<off_t>(0x24) ? GetValue<off_t>(0x24) + SUB_OFFSET : 0;
  mh2o_off = GetValue<off_t>(0x28) ? GetValue<off_t>(0x28) + SUB_OFFSET : 0;
  mftx_off = GetValue<off_t>(0x2c) ? GetValue<off_t>(0x2c) + SUB_OFFSET : 0;
}

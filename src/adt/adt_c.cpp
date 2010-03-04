#include "adt_c.h"

Adt_c::Adt_c(uint8_t *buffer, int64_t size) : ChunkReader_c(buffer, size) {
  Initialize();
}

void Adt_c::Initialize() {
  GetChunkById("RDHM", &mhdr_chunk_);
  GetChunkById("NICM", &mcin_chunk_);
  GetChunkById("XDMM", &mmdx_chunk_);
  GetChunkById("DIMM", &mmid_chunk_);
  GetChunkById("OMWM", &mwmo_chunk_);
  GetChunkById("DIWM", &mwid_chunk_);
  GetChunkById("FDDM", &mddf_chunk_);
  GetChunkById("FDOM", &modf_chunk_);

  InitMcnkChunks();

  raw_buffer().clear();
}

void Adt_c::InitMcnkChunks() {
  for (McinChunk_s::McnkIndex_t::iterator mcnk = mcin_chunk_.mcnk_index.begin();
         mcnk != mcin_chunk_.mcnk_index.end();
         ++mcnk) {
    mcin_chunk_.mcnk_list.push_back(McnkChunk_s());
    GetChunkByOffset(mcnk->mcnk_offset, &mcin_chunk_.mcnk_list.back());
  }
}

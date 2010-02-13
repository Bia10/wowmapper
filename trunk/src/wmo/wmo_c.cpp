#include "wmo_c.h"

Wmo_c::Wmo_c(uint8_t **buffer, const char *name)
    : raw_data_(*buffer),
      name_(name),
      mohd_chunk_(0xC, raw_data_),
      mogn_chunk_(NULL) {
  *buffer = NULL;

  Chunk_s skip_chunk;
  uint32_t chunk_offset = 0xC + mohd_chunk_.total_size();

  /* skip MOTX and MOMT */
  for(int i = 0; i < 2; i++) {
    skip_chunk = Chunk_s(chunk_offset, raw_data_, false);
    chunk_offset += skip_chunk.total_size();
  }

  mogn_chunk_ = new MognChunk_s(chunk_offset, raw_data_);
  chunk_offset += mogn_chunk_->total_size();

  /* skip MOGI, MOSB, MOPV, MOPT, MOPR, MOVV, MOVB, MOLT, MODS, */
  /*for(int i = 0; i < 9; i++) {
    skip_chunk = Chunk_s(chunk_offset, raw_data_, false);
    chunk_offset += skip_chunk.total_size();
  }*/

  /* read MODN here */
}

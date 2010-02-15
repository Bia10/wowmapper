#include "subwmo_c.h"

SubWmo_c::SubWmo_c(uint8_t **buffer, const char *name)
    : name_(name),
      mogp_chunk_(0xC, *buffer) {
  /* constant offset here: mogp seems to hold the size for the whole sub wmo */
  uint32_t chunk_offset = 0x58;
  mopy_chunk_ = MopyChunk_t(new MopyChunk_s(chunk_offset, *buffer));

  /* MOPY chunk contains triangle collision information */
  chunk_offset += mopy_chunk_->total_size();
  movi_chunk_ = MoviChunk_t(new MoviChunk_s(chunk_offset, *buffer));

  /* MOVI chunk contains actual vertex data for wmos */
  chunk_offset += movi_chunk_->total_size();
  movt_chunk_ = MovtChunk_t(new MovtChunk_s(chunk_offset, *buffer, mopy_chunk_));

  /* MONR chunk contains the corresponding normals */
  chunk_offset += movt_chunk_->total_size();
  monr_chunk_ = MonrChunk_t(new MonrChunk_s(chunk_offset, *buffer));

  delete [] *buffer;
  *buffer = NULL;     // invalidate buffer
}

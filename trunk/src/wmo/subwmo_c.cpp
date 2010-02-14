#include "subwmo_c.h"

SubWmo_c::SubWmo_c(uint8_t **buffer, const char *name)
    : name_(name),
      mogp_chunk_(0xC, *buffer),
      movi_chunk_(NULL),
      movt_chunk_(NULL),
      monr_chunk_(NULL) {
  uint32_t chunk_offset = 0x58;
  chunk_offset += Chunk_s(chunk_offset, *buffer, false).total_size();
  movi_chunk_ = new MoviChunk_s(chunk_offset, *buffer);

  chunk_offset += movi_chunk_->total_size();
  movt_chunk_ = new MovtChunk_s(chunk_offset, *buffer);

  chunk_offset += movt_chunk_->total_size();
  monr_chunk_ = new MonrChunk_s(chunk_offset, *buffer);

  delete [] *buffer;
  *buffer = NULL;     // invalidate buffer
}

SubWmo_c::~SubWmo_c() {
  SAFE_DELETE(movi_chunk_);
  SAFE_DELETE(movt_chunk_);
  SAFE_DELETE(monr_chunk_);
}

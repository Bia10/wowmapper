#include "chunk_c.h"

Chunk_c::Chunk_c(Buffer_t *buffer)
		: parent_(NULL), off_(0) {
  buffer_.swap(*buffer);
}

Chunk_c::Chunk_c(Chunk_c *parent, wm_off_t off)
    : parent_(parent), off_(off) {

}

Chunk_c::~Chunk_c() {
	parent_ = NULL;
	off_ = 0;
}

wm_off_t Chunk_c::GetCurOffset() const {
  if (parent_ != NULL) {
    return parent_->GetCurOffset() + off_;
  }
  return off_;
}

const Buffer_t& Chunk_c::GetBuffer() const {
  if (parent_ != NULL) {
    return parent_->GetBuffer();
  }
  return buffer_;
}

wm_size_t Chunk_c::GetSize() const {
  return GetValue<wm_size_t>(0, SIZE_OFFSET);
}

wm_off_t Chunk_c::GetOffsetToNext() const {
  // +0x8 because header id and data size must be considered
  return GetCurOffset()+GetSize()+DATA_OFFSET;
}

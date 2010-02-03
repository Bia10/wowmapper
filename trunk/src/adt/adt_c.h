#pragma once

#include <stdint.h>
#include "mhdrchunk_s.h"

/*! @brief Used to parse ADT files, just supply the buffer.
 *  @remark Adt_c dtor will destroy the buffer. */
class Adt_c {
 public:
  Adt_c(uint8_t *buffer);
  virtual ~Adt_c();

  const uint8_t* raw_data() const { return raw_data_; }
  const MhdrChunk_s& mhdr_chunk() const { return mhdr_chunk_; }

 private:
  uint8_t *raw_data_;
  MhdrChunk_s mhdr_chunk_;
};

#pragma once

#include <stdint.h>
#include "mhdrchunk_s.h"

#define MHDR_CHUNK_DATA_SIZE 0x40

class Adt_c {
 public:
  Adt_c(uint8_t *buffer);
  ~Adt_c();

 private:
  bool Initialize();

  MhdrChunk_s mhdr_chunk_;

  uint8_t *raw_data_;
};

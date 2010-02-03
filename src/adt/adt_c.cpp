#include "adt_c.h"
#include <string.h>
#include <stdio.h>

#define MHDR_OFFSET 0xC

Adt_c::Adt_c(uint8_t *buffer)
    : raw_data_(buffer),
      mhdr_chunk_(MHDR_OFFSET, raw_data_) {
  int i = 0;
}

Adt_c::~Adt_c() {
  delete [] raw_data_;
}

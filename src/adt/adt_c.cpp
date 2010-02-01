#include "adt_c.h"
#include <string.h>
#include <stdio.h>

Adt_c::Adt_c(uint8_t *buffer)
    : raw_data_(buffer),
      mhdr_chunk_(0xC, raw_data_) {
}

Adt_c::~Adt_c() {
  delete [] raw_data_;
}

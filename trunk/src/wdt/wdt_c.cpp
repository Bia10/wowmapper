#include "wdt_c.h"
#include <stdio.h>

#define MPHD_OFFSET 0xC
#define MAIN_OFFSET 0x34

Wdt_c::Wdt_c(uint8_t *buffer, const char *name)
    : raw_data_(buffer),
      name_(name),
      mphd_chunk_(MPHD_OFFSET, raw_data_),
      main_chunk_(MAIN_OFFSET, raw_data_) {
  int num_adt = 0;
  for(int y = 0; y < 64; y++) {
    for(int x = 0; x < 64; x++) {
      if(!(main_chunk_.map_tile[y][x].flags & 2)) {
        printf("%s_%02d_%02d\n", name_.c_str(), x, y);
        num_adt++;
      }
    }
  }

  printf("ADTs #%d\n", num_adt);
}

Wdt_c::~Wdt_c() {
  delete [] raw_data_;
}

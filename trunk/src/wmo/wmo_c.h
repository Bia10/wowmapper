#pragma once

#include "mohdchunk_s.h"
#include "mognchunk_s.h"

class Wmo_c {
 public:
  Wmo_c(uint8_t **buffer, const char *name);

  const uint8_t* raw_data() const { return raw_data_; }

 private:
  uint8_t *raw_data_;
  std::string name_;
  MohdChunk_s mohd_chunk_;
  MognChunk_s *mogn_chunk_;
};

#pragma once

#include <stdint.h>
#include <string>
#include "mphdchunk_s.h"
#include "mainchunk_s.h"

class Wdt_c {
 public:
  Wdt_c(uint8_t *buffer, const char *name);
  virtual ~Wdt_c();

 private:
  uint8_t *raw_data_;
  std::string name_;
  MphdChunk_s mphd_chunk_;
  MainChunk_s main_chunk_;
};

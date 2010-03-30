#pragma once

#include "mohdchunk_s.h"
#include "mognchunk_s.h"
#include "mogichunk_s.h"
#include "modschunk_s.h"
#include "moddchunk_s.h"
#include "../../mpqhandler_c.h"

class Wmo_c : public Chunk_c {
 public:
  Wmo_c(const uint8_t *buffer, uint32_t length, MpqHandler_c &mpq_h);

 private:
  MohdChunk_s mohd_;
  MognChunk_s mogn_;
  MogiChunk_s mogi_;
  ModsChunk_s mods_;
  ModdChunk_s modd_;
};

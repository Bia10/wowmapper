#pragma once

#include "mohdchunk_s.h"
#include "mognchunk_s.h"
#include "mogichunk_s.h"
#include "modschunk_s.h"
#include "moddchunk_s.h"
#include "subwmo_c.h"
#include "../model_c.h"
#include "../../mpqhandler_c.h"

class Wmo_c : public Model_c {
 public:
  Wmo_c(Buffer_t *buffer, std::string wmo_name, MpqHandler_c &mpq_h);

  const Indices32_t& indices() { return indices_; }
  const Points_t& vertices() { return vertices_; }
  const Points_t& normals() { return normals_; }
  const Indices32_t& colors() { return colors_; }

 private:
  MohdChunk_s mohd_;
  MognChunk_s mogn_;
  MogiChunk_s mogi_;
  ModsChunk_s mods_;
  ModdChunk_s modd_;

  Indices32_t indices_;
  Points_t vertices_;
  Points_t normals_;
  Indices32_t colors_;
};

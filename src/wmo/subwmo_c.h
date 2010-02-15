#pragma once

#include "mogpchunk_s.h"
#include "movichunk_s.h"
#include "movtchunk_s.h"
#include "monrchunk_s.h"
#include "mopychunk_s.h"

class SubWmo_c {
 public:
  SubWmo_c(uint8_t **buffer, const char *name);

  const MopyChunk_t& mopy_chunk() const { return mopy_chunk_; }
  const MoviChunk_t& movi_chunk() const { return movi_chunk_; }
  const MovtChunk_t& movt_chunk() const { return movt_chunk_; }
  const MonrChunk_t& monr_chunk() const { return monr_chunk_; }

  const std::string& name() const { return name_; }

 private:
  std::string name_;
  MogpChunk_s mogp_chunk_;
  MopyChunk_t mopy_chunk_;
  MoviChunk_t movi_chunk_;
  MovtChunk_t movt_chunk_;
  MonrChunk_t monr_chunk_;
};

#pragma once

#include "mogpchunk_s.h"
#include "movichunk_s.h"
#include "movtchunk_s.h"
#include "monrchunk_s.h"

class SubWmo_c {
 public:
  SubWmo_c(uint8_t **buffer, const char *name);
  ~SubWmo_c();

  const MoviChunk_s *movi_chunk() const { return movi_chunk_; }
  const MovtChunk_s *movt_chunk() const { return movt_chunk_; }
  const MonrChunk_s *monr_chunk() const { return monr_chunk_; }
  const std::string& name() const { return name_; }

 private:
  std::string name_;
  MogpChunk_s mogp_chunk_;
  MoviChunk_s *movi_chunk_;
  MovtChunk_s *movt_chunk_;
  MonrChunk_s *monr_chunk_;
};

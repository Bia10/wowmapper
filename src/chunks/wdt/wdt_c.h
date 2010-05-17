#pragma once

#include "mphdchunk_s.h"
#include "mainchunk_s.h"

/*! \brief Used to read WDT files and retrieve ADTs. */
class Wdt_c : public Chunk_c {
 public:
  Wdt_c(Buffer_t *buf, const std::string &name);
  const AdtPos_t& adt_list() const { return list_; }

 private:
  void GetList();

  std::string name_;
  AdtPos_t list_;

  MphdChunk_s mphd_;
  MainChunk_s main_;
};

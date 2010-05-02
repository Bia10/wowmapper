#pragma once

#include "mphdchunk_s.h"
#include "mainchunk_s.h"

typedef std::list<std::string> NamesList_t;

/*! \brief Used to read WDT files and retrieve ADTs. */
class Wdt_c : public Chunk_c {
 public:
  Wdt_c(Buffer_t *buf, const std::string &name);
  const NamesList_t& names() const { return names_; }

 private:
  void GenerateAdtNames();

  std::string name_;
  NamesList_t names_;

  MphdChunk_s mphd_;
  MainChunk_s main_;
};

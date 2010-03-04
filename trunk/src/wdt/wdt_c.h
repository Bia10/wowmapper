#pragma once

#include "../chunkreader_c.h"
#include "mphdchunk_s.h"
#include "mainchunk_s.h"

typedef std::list<std::string> AdtNames_t;

/*! \brief Used to read WDT files and retrieve ADTs. */
class Wdt_c : protected ChunkReader_c {
 public:
  Wdt_c(uint8_t *buffer, int64_t size, const char *name);

  const MainChunk_s& main_chunk() const { return main_chunk_; }
  const AdtNames_t& adt_names() const { return adt_names_; }

 private:
  void Initialize();
  void GenerateAdtNames();

  std::string map_name_;
  AdtNames_t adt_names_;

  MphdChunk_s mphd_chunk_;
  MainChunk_s main_chunk_;
};

#pragma once

#include <stdint.h>
#include <string>
#include <list>
#include "mphdchunk_s.h"
#include "mainchunk_s.h"
#include "../mpqhandler_c.h"

class Adt_c;

typedef std::list<std::string> AdtNames_t;
typedef std::list<Adt_c*> AdtList_t;

/*! @brief Used to read WDT files and retrieve ADTs. */
class Wdt_c {
 public:
  Wdt_c(uint8_t *buffer, const char *name);
  virtual ~Wdt_c();

  void LoadAdts(MpqHandler_c &mpq_handler, AdtList_t *outAdtList) const;

 private:
  void GenerateAdtNames();

  uint8_t *raw_data_;
  std::string map_name_;
  MphdChunk_s mphd_chunk_;
  MainChunk_s main_chunk_;
  AdtNames_t adt_names_;
};

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
  Wdt_c(uint8_t **buffer, const char *name);
  virtual ~Wdt_c();

  /*! @brief Loads all ADTs that are known through intialization of this WDT.
   *
   * @param mpq_handler MpqHandle_c reference needed to load ADTs from MPQ
   * @param outAdtList Returns a list of ADTs
   */
  void LoadAdts(MpqHandler_c &mpq_handler,
                AdtList_t *outAdtList,
                uint32_t count = 0xffffffff,
                uint32_t offset = 0) const;
  void UnloadAdts(AdtList_t *adtList) const;

 private:
  void GenerateAdtNames();

  std::string map_name_;
  MphdChunk_s mphd_chunk_;
  MainChunk_s main_chunk_;
  AdtNames_t adt_names_;
};

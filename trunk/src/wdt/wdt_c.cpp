#include "wdt_c.h"
#include <iostream>
#include <sstream>
#include "../adt/adt_c.h"


#define MPHD_OFFSET 0xC
#define MAIN_OFFSET 0x34

Wdt_c::Wdt_c(uint8_t *buffer, const char *name)
    : raw_data_(buffer),
      map_name_(name),
      mphd_chunk_(MPHD_OFFSET, raw_data_),
      main_chunk_(MAIN_OFFSET, raw_data_) {
  GenerateAdtNames();
}

Wdt_c::~Wdt_c() {
  delete [] raw_data_;
}

void Wdt_c::GenerateAdtNames() {
  std::stringstream tile_name;

  /* go through all map tiles and retrieve adt positions and their names */
  for (int y = 0; y < 64; y++) {
    for (int x = 0; x < 64; x++) {
      if(main_chunk_.map_tile[y][x].flags) {
        /* create adt filename and push it to the vector */
        tile_name << map_name_ << "_" << x << "_" << y << ".adt";
        adt_names_.push_back(tile_name.str());
        tile_name.clear();
      }
    }
  }

  std::cout << "Retrieved " << adt_names_.size() << " map tiles." << std::endl;
}

void Wdt_c::LoadAdts(MpqHandler_c &mpq_handler, AdtList_t *outAdtList) const {
  outAdtList->clear();
  uint8_t *file_buf = NULL;

  for(AdtNames_t::const_iterator name = adt_names_.begin();
      name != adt_names_.end();
      ++name) {
    /* load file from mpq and push adts to list */
    mpq_handler.LoadFileByName(name->c_str(), &file_buf);
    outAdtList->push_back(new Adt_c(file_buf));
  }
}

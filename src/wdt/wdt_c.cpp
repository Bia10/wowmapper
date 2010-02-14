#include "wdt_c.h"
#include <iostream>
#include <sstream>
#include "../adt/adt_c.h"

#define MPHD_OFFSET 0xC
#define MAIN_OFFSET 0x34

Wdt_c::Wdt_c(uint8_t **buffer, const char *name)
    : map_name_(name),
      mphd_chunk_(MPHD_OFFSET, *buffer),
      main_chunk_(MAIN_OFFSET, *buffer) {
  delete [] *buffer;
  *buffer = NULL;       // invalidate buffer
  GenerateAdtNames();
}

Wdt_c::~Wdt_c() {

}

void Wdt_c::GenerateAdtNames() {
  std::stringstream tile_name;

  /* go through all map tiles and retrieve adt positions and their names */
  for (int y = 0; y < 64; y++) {
    for (int x = 0; x < 64; x++) {
      if(main_chunk_.map_tile[y][x].flags & 1) {
        /* create adt filename and push it to the vector */
        tile_name << map_name_ << "_" << x << "_" << y << ".adt";
        adt_names_.push_back(tile_name.str());
        /* clear string stream */
        tile_name.str("");
        tile_name.clear();
      }
    }
  }

  std::cout << "Retrieved " << adt_names_.size() << " adt names." << std::endl;
}

void Wdt_c::LoadAdts(MpqHandler_c &mpqHandler,
                     AdtList_t *outAdtList,
                     uint32_t count,
                     uint32_t offset) const {
  uint32_t counter = 0;
  int64_t mem_size = 0;

  uint8_t *file_buf = NULL;

  for(AdtNames_t::const_iterator name = adt_names_.begin();
      name != adt_names_.end();
      ++name, counter++) {
    if(counter < offset || counter > offset+count) continue;

      /* print adt names that are loaded */
      std::cout << "Load ADT (" << counter << "/" << adt_names_.size() << "): ";
      std::cout << *name << std::endl;

      /* load file from mpq and push adts to list */
      mem_size += mpqHandler.LoadFileByName(name->c_str(), &file_buf);
      outAdtList->push_back(new Adt_c(&file_buf, &mpqHandler));
  }

  std::cout << mem_size / (1024 * 1024) << " Mb loaded." << std::endl;
}

void Wdt_c::UnloadAdts(AdtList_t *adtList) const {
  for(AdtList_t::const_iterator adt = adtList->begin();
      adt != adtList->end();
      ++adt) {
    delete *adt;
  }

  adtList->clear();
}


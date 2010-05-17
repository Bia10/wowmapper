#include "wdt_c.h"

Wdt_c::Wdt_c(Buffer_t *buf, const std::string &name)
    : Chunk_c(buf),
      name_(name),
      mphd_(this, 0xc),
      main_(this, mphd_.GetOffsetToNext()) {
  GetList();
}

void Wdt_c::GetList() {
  // go through all map tiles and retrieve adt positions and their names
  for (int y = 0; y < 64; y++) {
    for (int x = 0; x < 64; x++) {
      if(main_.map_tiles[y*64+x].flags & 1) {
        AdtPos_s pos = { x, y };
        list_.push_back(pos);
      }
    }
  }

  std::cout << "ADTs: " << list_.size() << std::endl;
}

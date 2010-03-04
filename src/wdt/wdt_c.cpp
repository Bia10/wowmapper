#include "wdt_c.h"
#include "../adt/adt_c.h"

Wdt_c::Wdt_c(uint8_t *buffer, int64_t size, const char *name)
    : ChunkReader_c(buffer, size),
      map_name_(name) {
  Initialize();
  GenerateAdtNames();
}

void Wdt_c::Initialize() {
  GetChunkById("DHPM", &mphd_chunk_);
  GetChunkById("NIAM", &main_chunk_);
}

void Wdt_c::GenerateAdtNames() {
  // go through all map tiles and retrieve adt positions and their names
  for (int y = 0; y < 64; y++) {
    for (int x = 0; x < 64; x++) {
      if(main_chunk_.map_tiles[y*64+x].flags & 1) {
        // create adt filename and push it to the vector
        std::stringstream tile_name;
        tile_name << map_name_ << "_" << x << "_" << y << ".adt";
        adt_names_.push_back(tile_name.str()); // add name to adt list
      }
    }
  }

  std::cout << "ADTs: " << adt_names_.size() << std::endl;
}

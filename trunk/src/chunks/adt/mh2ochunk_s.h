#pragma once

#include "../chunk_c.h"

/*! \brief MH2O chunk.
 *  http://www.madx.dk/wowdev/wiki/index.php?title=ADT/v18#MH2O_chunk */
struct Mh2oChunk_s : public Chunk_c {
  struct Mh2oInfo_s {
    uint16_t liquid_type;
    uint16_t flags;
    float height_level_1;
    float height_level_2;
    uint8_t x_offset;
    uint8_t y_offset;
    uint8_t width;
    uint8_t height;
    uint32_t mask_offset;
    uint32_t heightmap_offset;
  };
  typedef std::vector<Mh2oInfo_s> Mh2oInfo_t;

  Mh2oInfo_t mh2o_info;

  Mh2oChunk_s(Chunk_c *parent) : Chunk_c(parent) { }

 protected:
  virtual void Initialize() {
    mh2o_info.reserve(256);

    Mh2oHeader_t mh2o_header(256);
    CopyDataBlock(buffer_, 256, &mh2o_header);

    for (Mh2oHeader_t::iterator mh2o = mh2o_header.begin();
         mh2o != mh2o_header.end();
         ++mh2o) {
      if (mh2o->layer_count) {
        mh2o_info.push_back(Mh2oInfo_s());
        mh2o_info.back() = GetField<Mh2oInfo_s>(mh2o->info_offset);
      }
    }
  }

 private:
  struct Mh2oHeader_s {
    uint32_t info_offset;
    uint32_t layer_count;
    uint32_t render_mask_offset;
  };
  typedef std::vector<Mh2oHeader_s> Mh2oHeader_t;
};

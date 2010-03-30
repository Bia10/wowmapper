#pragma once

#include "../chunk_c.h"

/*! \brief MH2O chunk.
 *  http://www.madx.dk/wowdev/wiki/index.php?title=ADT/v18#MH2O_chunk */
struct Mh2oChunk_s : public Chunk_c {
  struct Mh2oHeader_s {
    uint32_t info_offset;
    uint32_t layer_count;
    uint32_t render_mask_offset;
  };
  typedef std::vector<Mh2oHeader_s> Mh2oHeader_t;

  struct Mh2oInfo_s {
    uint16_t type;
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

  struct Mh2oHeights_s {
    uint64_t mask;
    uint32_t x, y, w, h;
    std::vector<float> val;
  };
  typedef std::vector<Mh2oHeights_s> Mh2oHeights_t;


  Mh2oHeader_t header;
  Mh2oInfo_t info;
  Mh2oHeights_t heights;

  Mh2oChunk_s(Chunk_c *parent)
    : Chunk_c(parent), header(256), info(256), heights(256) { }

 protected:
  virtual void Initialize() {
    CopyDataBlock(buffer_, 0, 256, &header);

    for (int i = 0; i < 256; i++) {
      if (header[i].layer_count) {
        info[i] = GetField<Mh2oInfo_s>(header[i].info_offset);
        if ((info[i].type & 1) != 1) { return; }

        // assign height properties
        heights[i].mask = GetField<uint64_t>(header[i].render_mask_offset);
        heights[i].w = info[i].width;
        heights[i].h = info[i].height;
        heights[i].x = info[i].x_offset;
        heights[i].y = info[i].y_offset;

        // a 3x3 grid is made up from 4x4 heights (their corners)
        uint32_t num_h = (heights[i].w+1)*(heights[i].h+1);

        // get heights from heightmap or from height level
        if (info[i].heightmap_offset) {
          heights[i].val.resize(num_h);
          CopyDataBlock(buffer_, info[i].heightmap_offset, num_h, &heights[i].val);
        } else {
          heights[i].val.assign(num_h, info[i].height_level_1);
        }
      }
    }
  }
};

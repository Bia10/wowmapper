#pragma once

#include "../chunk_c.h"

/*! \brief MH2O: Map Chunk H2O (water). */
struct Mh2oChunk_s : public Chunk_c {
  struct Mh2oInfo_s {
    uint16_t type;
    uint16_t flags;
    float height_level_1;
    float height_level_2;
    uint8_t x_offset;
    uint8_t y_offset;
    uint8_t width;
    uint8_t height;
    uint32_t mask2_offset;
    uint32_t heightmap_offset;
  };
  typedef std::vector<Mh2oInfo_s> Mh2oInfo_t;
  typedef std::vector<std::vector<float> > Heightmaps_t;

  struct Mh2oContent_s {
    Indices64_t masks;
    Mh2oInfo_t infos;
    Heightmaps_t heightmaps;
    uint32_t layers;
  };
  typedef std::vector<Mh2oContent_s> Mh2oContent_t;

  Mh2oContent_t content;

  Mh2oChunk_s(Chunk_c *parent)
    : Chunk_c(parent), content(256), header(256) { }

 protected:
  virtual void LateInit() {
    CopyDataBlock(buffer_, 0, 256, &header);

    for (int i = 0; i < 256; i++) {
      // resize mh2o content arrays to fit the information
      uint32_t lcount = header[i].layer_count;
      content[i].layers = lcount;
      content[i].infos.resize(lcount);
      content[i].masks.resize(lcount);
      content[i].heightmaps.resize(lcount);

      // retrieve render mask and mh2o info by going through the layers
      for (uint32_t l = 0; l < lcount; l++) {
        // get offsets and ultimately render mask and information
        uint32_t mask_off = header[i].mask_offset+l*sizeof(uint64_t);
        uint32_t info_off = header[i].info_offset+l*sizeof(Mh2oInfo_s);
        content[i].masks[l] = GetField<uint64_t>(mask_off);
        content[i].infos[l] = GetField<Mh2oInfo_s>(info_off);

        const Mh2oInfo_s &info = content[i].infos[l];
        uint32_t hm_off = info.heightmap_offset;
        uint32_t num_h = (info.width+1)*(info.height+1);  // max 9x9 array

        // if we have a heightmap offset, use it otherwise assign level1 height
        if (hm_off) {
          content[i].heightmaps[l].resize(num_h);
          CopyDataBlock(buffer_, hm_off, num_h, &content[i].heightmaps[l]);
        } else {
          content[i].heightmaps[l].assign(num_h, info.height_level_1);
        }
      }
    }
  }
 private:
  struct Mh2oHeader_s {
    uint32_t info_offset;
    uint32_t layer_count;
    uint32_t mask_offset;
  };
  typedef std::vector<Mh2oHeader_s> Mh2oHeader_t;

  Mh2oHeader_t header;
};

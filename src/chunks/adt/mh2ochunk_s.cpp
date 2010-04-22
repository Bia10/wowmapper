#include "mh2ochunk_s.h"

Mh2oChunk_s::Mh2oChunk_s(Chunk_c *parent, off_t off)
    : Chunk_c(parent, off), header(256), content(256) {
  CopyVector(GetBuffer(), GetCurOffset()+DATA_OFFSET, 256, &header);

  for (int i = 0; i < 256; i++) {
    // resize mh2o content arrays to fit the information
    size_t num_layers = header[i].num_layers;
    content[i].num_layers = num_layers;
    content[i].infos.resize(num_layers);
    content[i].masks.resize(num_layers);
    content[i].heightmaps.resize(num_layers);

    // retrieve render mask and mh2o info by going through the layers
    for (uint32_t l = 0; l < num_layers; l++) {
      // get offsets and ultimately render mask and information
      uint32_t mask_off = header[i].mask_off+l*sizeof(uint64_t);
      uint32_t info_off = header[i].info_off+l*sizeof(Mh2oInfo_s);
      content[i].masks[l] = GetValue<uint64_t>(mask_off);
      content[i].infos[l] = GetValue<Mh2oInfo_s>(info_off);

      const Mh2oInfo_s &info = content[i].infos[l];
      uint32_t hm_off = info.heightmap_off;
      uint32_t num_h = (info.width+1)*(info.height+1);  // max 9x9 array

      // if we have a heightmap offset, use it otherwise assign level1 height
      if (hm_off) {
        content[i].heightmaps[l].resize(num_h);
        hm_off += GetCurOffset() + DATA_OFFSET;
        CopyVector(GetBuffer(), hm_off, num_h, &content[i].heightmaps[l]);
      } else {
        content[i].heightmaps[l].assign(num_h, info.height_level_1);
      }
    }
  }
}

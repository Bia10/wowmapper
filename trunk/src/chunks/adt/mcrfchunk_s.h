#pragma once

#include "../chunk_c.h"

/*! \brief MCRF chunk
 *  http://www.madx.dk/wowdev/wiki/index.php?title=ADT#MCNK_sub-chunk:_MCRF_Map_Chunk_ReFerences */
struct McrfChunk_s : public Chunk_c {
  Indices32_t doodad_offsets;
  Indices32_t wmo_offsets;

  void SetNumRefs(uint32_t num_doodad_refs, uint32_t num_wmo_refs) {
    num_doodad_refs_ = num_doodad_refs;
    num_wmo_refs_ = num_wmo_refs;
  }

  McrfChunk_s(Chunk_c *parent)
      : Chunk_c(parent), num_doodad_refs_(0), num_wmo_refs_(0) {

  }

 protected:
  virtual void LateInit() {
    Indices32_t offsets(num_doodad_refs_ + num_wmo_refs_);
    CopyDataBlock(buffer_, &offsets);

    doodad_offsets.assign(offsets.begin(), offsets.begin()+num_doodad_refs_);
    wmo_offsets.assign(offsets.begin()+num_doodad_refs_, offsets.end());
  }

 private:
  uint32_t num_doodad_refs_;
  uint32_t num_wmo_refs_;
};

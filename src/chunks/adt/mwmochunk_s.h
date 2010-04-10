#pragma once

#include "../chunk_c.h"

/*! \brief MWMO chunk.
 *  http://www.madx.dk/wowdev/wiki/index.php?title=ADT/v18#MWMO_chunk */
struct MwmoChunk_s : public Chunk_c {
  std::string wmo_names;

  MwmoChunk_s(Chunk_c *parent) : Chunk_c(parent) { }

 protected:
  virtual void LateInit() {
    wmo_names.assign(buffer_.begin(), buffer_.end());
  }
};

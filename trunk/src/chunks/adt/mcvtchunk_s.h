#pragma once

#include "../chunk_c.h"

/*! \brief MCVT chunk
 *  \url http://www.madx.dk/wowdev/wiki/index.php?title=ADT/v18#MCVT_sub-chunk */
struct McvtChunk_s : public Chunk_c {
  std::vector<float> heights;

  McvtChunk_s(Chunk_c *parent) : Chunk_c(parent), heights(145) { }

 protected:
  virtual void Initialize() { CopyDataBlock(buffer_, &heights); }
};

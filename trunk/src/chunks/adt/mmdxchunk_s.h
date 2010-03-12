#pragma once

#include "../chunk_c.h"

/*! \brief MMDX chunk.
 *  http://www.madx.dk/wowdev/wiki/index.php?title=ADT/v18#MMDX_chunk */
struct MmdxChunk_s : public Chunk_c {
  std::string m2_names;

  MmdxChunk_s(Chunk_c *parent) : Chunk_c(parent) { }

 protected:
  virtual void Initialize() {
    m2_names.assign(buffer_.begin(), buffer_.end());
  }
};

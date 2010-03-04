#pragma once

#include "mcnkchunk_s.h"

/*! \brief MCIN chunk */
struct McinChunk_s : Chunk_s {
  struct McnkIndex_s {
    uint32_t mcnk_offset;
    uint32_t size;
    uint32_t unused_flags;
    uint32_t unused_async_id;
  };

  typedef std::vector<McnkIndex_s> McnkIndex_t;
  typedef std::list<McnkChunk_s> McnkList_t;

  McnkIndex_t mcnk_index;
  McnkList_t mcnk_list;   // has to be set from outside


  McinChunk_s() : Chunk_s(), mcnk_index(256) { }

  virtual void Initialize() {
    CopyData(mcnk_index.data());
  }
};

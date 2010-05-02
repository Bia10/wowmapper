#pragma once

#include "../chunk_c.h"

/*! \brief MCIN: Map Chunk Index. Contains 256 entries for offests to every
 *         map chunk in this ADT. */
struct McinChunk_s : public Chunk_c {
  struct McnkInfo_s {
    off_t mcnk_off;         //!< absolute offset
    size_t size;            //!< the size of the MCNK chunk, this is refering to
    uint32_t unused_flags;  // these two are always 0, only set in the client
    uint32_t unused_async;
  };
  typedef std::vector<McnkInfo_s> McnkInfos_t;

  McnkInfos_t mcnk_info;     //!< contains 256 (16*16) MCNK info entries


  McinChunk_s(Chunk_c *parent, off_t off)
      : Chunk_c(parent, off), mcnk_info(256) {
    CopyVector(GetBuffer(), GetCurOffset()+DATA_OFFSET, 256, &mcnk_info);
  }
};



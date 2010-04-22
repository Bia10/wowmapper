#pragma once

#include "../chunk_c.h"

/*! \brief MCIN: Map Chunk Index. Contains 256 entries for offests to every
 *         map chunk in this ADT. */
struct McinChunk_s : public Chunk_c {
  struct McnkInfo_s;
  typedef std::vector<McnkInfo_s> McnkInfos_t;

  McnkInfos_t mcnk_info;     //!< contains 256 (16*16) MCNK info entries

	McinChunk_s(Chunk_c *parent, off_t off);
};

struct McinChunk_s::McnkInfo_s {
  off_t mcnk_off;         //!< absolute offset
  size_t size;            //!< the size of the MCNK chunk, this is refering to
  uint32_t unused_flags;  // these two are always 0, only set in the client
  uint32_t unused_async;
};

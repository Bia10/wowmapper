#pragma once

#include "mcinchunk_s.h"

const static uint32_t SUB_REL_OFFSET = 0x14; // relative offset for sub chunks

/*! \brief MHDR chunk.
 *  \url http://www.madx.dk/wowdev/wiki/index.php?title=ADT/v18#MHDR_chunk */
struct MhdrChunk_s : public Chunk_c {
	McinChunk_s mcin;

	MhdrChunk_s(Chunk_c *parent) : Chunk_c(parent), mcin(this) { }

 protected:
	virtual void Initialize() {
    uint32_t chunk_offset = GetField<uint32_t>(0x04) + SUB_REL_OFFSET;
    parent_->GetSubChunk(chunk_offset, &mcin);
  }
};

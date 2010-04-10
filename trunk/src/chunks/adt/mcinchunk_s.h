#pragma once

#include "mcnkchunk_s.h"

/*! \brief MCIN chunk.
 *  http://www.madx.dk/wowdev/wiki/index.php?title=ADT#MCIN_chunk */
struct McinChunk_s : public Chunk_c {
	typedef std::vector<McnkChunk_s> McnkChunks_t;

	McnkChunks_t mcnks;

	McinChunk_s(Chunk_c *parent)
	    : Chunk_c(parent),
	      mcnks(256, McnkChunk_s(this)),
	      mcnk_info(256) {

	}

 protected:
	virtual void LateInit() {
	  CopyDataBlock(buffer_, &mcnk_info);

	  // get mcnks by mcnk info
	  for (int i = 0; i < 256; i++) {
	    parent_->parent()->GetSubChunk(mcnk_info[i].mcnk_offset, &mcnks[i]);
	  }

	  mcnk_info.clear(); // clear mcnk info, not needed anymore
	}

 private:
	struct McnkInfo_s {
    uint32_t mcnk_offset;   //!< absolute offset
    uint32_t size;          //!< the size of the MCNK chunk, this is refering to
    uint32_t unused_flags;  //!< these two are always 0, only set in the client
    uint32_t unused_async;
  };
	typedef std::vector<McnkInfo_s> McnkInfo_t;

	McnkInfo_t mcnk_info;     //!< contains 256 MCNK entries (16*16)
};

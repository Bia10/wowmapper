#pragma once

#include "mcinchunk_s.h"
#include "mmdxchunk_s.h"
#include "mmidchunk_s.h"
#include "mwmochunk_s.h"
#include "mwidchunk_s.h"
#include "mddfchunk_s.h"
#include "modfchunk_s.h"
#include "mh2ochunk_s.h"

const static uint32_t SUB_REL_OFFSET = 0x14; // relative offset for sub chunks

/*! \brief MHDR chunk.
 *  \url http://www.madx.dk/wowdev/wiki/index.php?title=ADT/v18#MHDR_chunk */
struct MhdrChunk_s : public Chunk_c {
	McinChunk_s mcin; //!< map chunks
	MmdxChunk_s mmdx; //!< model filenames
  MmidChunk_s mmid; //!< model ids (offsets to filenames)
  MwmoChunk_s mwmo; //!< wmo filenames
  MwidChunk_s mwid; //!< wmo ids (offsets to filenames)
  MddfChunk_s mddf; //!< doodad information
  ModfChunk_s modf; //!< wmo information
  Mh2oChunk_s mh2o; //!< water information

	MhdrChunk_s(Chunk_c *parent)
      : Chunk_c(parent), mcin(this), mmdx(this), mmid(this), mwmo(this),
        mwid(this), mddf(this), modf(this), mh2o(this) { }

 protected:
	virtual void LateInit() {
    parent_->GetSubChunk(GetField<uint32_t>(0x04) + SUB_REL_OFFSET, &mcin);
    parent_->GetSubChunk(GetField<uint32_t>(0x0c) + SUB_REL_OFFSET, &mmdx);
    parent_->GetSubChunk(GetField<uint32_t>(0x10) + SUB_REL_OFFSET, &mmid);
    parent_->GetSubChunk(GetField<uint32_t>(0x14) + SUB_REL_OFFSET, &mwmo);
    parent_->GetSubChunk(GetField<uint32_t>(0x18) + SUB_REL_OFFSET, &mwid);
    parent_->GetSubChunk(GetField<uint32_t>(0x1c) + SUB_REL_OFFSET, &mddf);
    parent_->GetSubChunk(GetField<uint32_t>(0x20) + SUB_REL_OFFSET, &modf);
    // check if mh2o chunk exists
    uint32_t mh2o_off = GetField<uint32_t>(0x28);
    if (mh2o_off) { parent_->GetSubChunk(mh2o_off + SUB_REL_OFFSET, &mh2o); }
  }
};

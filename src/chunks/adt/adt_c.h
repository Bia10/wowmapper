#pragma once

#include "mhdrchunk_s.h"
#include "mmdxchunk_s.h"
#include "mmidchunk_s.h"
#include "mwmochunk_s.h"
#include "mwidchunk_s.h"
#include "mddfchunk_s.h"
#include "modfchunk_s.h"
#include "mh2ochunk_s.h"
#include "../../mpqhandler_c.h"

/*! \brief ADT files. */
class Adt_c : public Chunk_c {
 public:
	Adt_c(const uint8_t *buffer, uint32_t length, MpqHandler_c &mpq_h);

 private:
	MhdrChunk_s mhdr_;
	MmdxChunk_s mmdx_;
	MmidChunk_s mmid_;
	MwmoChunk_s mwmo_;
	MwidChunk_s mwid_;
	MddfChunk_s mddf_;
	ModfChunk_s modf_;
	Mh2oChunk_s mh2o_;
};

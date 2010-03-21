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

	const Points_t& vertices() const { return vertices_; }
	const Points_t& normals() const { return normals_; }
	const Indices32_t& indices() const { return indices_; }

 private:
	MhdrChunk_s mhdr_;
	MmdxChunk_s mmdx_;
	MmidChunk_s mmid_;
	MwmoChunk_s mwmo_;
	MwidChunk_s mwid_;
	MddfChunk_s mddf_;
	ModfChunk_s modf_;
	Mh2oChunk_s mh2o_;

	Points_t vertices_;
	Points_t normals_;
	Indices32_t indices_;
};

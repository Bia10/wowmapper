#pragma once

#include "mhdrchunk_c.h"

/*! \brief ADT files. */
class Adt_c : public Chunk_c {
 public:
	Adt_c(const uint8_t *buffer, uint32_t length);

 private:
	MhdrChunk_s mhdr_;
};

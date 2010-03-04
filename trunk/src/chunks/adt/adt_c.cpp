#include "adt_c.h"

Adt_c::Adt_c(const uint8_t *buffer, uint32_t length)
		: Chunk_c(buffer, length),
			mhdr_(this) {
	GetSubChunk("RDHM", 4, &mhdr_);
}

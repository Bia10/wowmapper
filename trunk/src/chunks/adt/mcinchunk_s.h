#pragma once

#include "../chunk_c.h"

struct McinChunk_s : public Chunk_c {
	McinChunk_s(Chunk_c *parent) : Chunk_c(parent) { }
};

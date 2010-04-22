#pragma once

#include "../chunk_c.h"

/*! \brief MWMO: Map Chunk WMO Names. */
struct MwmoChunk_s : public Chunk_c {
  std::string wmo_names;

  MwmoChunk_s(Chunk_c *parent, off_t off);
};

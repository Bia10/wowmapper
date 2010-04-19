#pragma once

#include "../chunk_c.h"

/*! \brief MWMO: Map Chunk WMO Names. */
struct MwmoChunk_s : public Chunk_c {
  std::string wmo_names;

  MwmoChunk_s(Chunk_c *parent) : Chunk_c(parent) { }

 protected:
  virtual void LateInit() {
    wmo_names.assign(buffer_.begin(), buffer_.end());
  }
};

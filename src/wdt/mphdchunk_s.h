#pragma once

#include "../chunk_s.h"

/*! \brief MPHD chunk. */
struct MphdChunk_s : Chunk_s {
  uint32_t flags;

  MphdChunk_s() : Chunk_s() {
    // initialize chunk fields
    field_offsets_.insert(FieldOffset_t("flags", DATA_OFFSET + 0x8));
  }

  virtual void Initialize() {
    // assign field values
    AssignValue("flags", &flags);
  }
};

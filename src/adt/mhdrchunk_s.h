#pragma once

#include "mcinchunk_s.h"
//#include "mwmochunk_s.h"
//#include "modfchunk_s.h"

/*! @brief MHDR chunk */
struct MhdrChunk_s : Chunk_s {
  uint32_t flags;
  uint32_t mcin_offset;
  uint32_t mwmo_offset;
  uint32_t modf_offset;


  MhdrChunk_s() : Chunk_s() {
    // initialize chunk fields
    field_offsets_.insert(FieldOffset_t("flags",        DATA_OFFSET + 0x00));
    field_offsets_.insert(FieldOffset_t("mcin_offset",  DATA_OFFSET + 0x04));
    field_offsets_.insert(FieldOffset_t("mwmo_offset",  DATA_OFFSET + 0x14));
    field_offsets_.insert(FieldOffset_t("modf_offset",  DATA_OFFSET + 0x20));
  }

  virtual void Initialize() {
    // assign field values
    AssignValue("flags", &flags);
    AssignValue("mcin_offset", &mcin_offset);
    AssignValue("mwmo_offset", &mwmo_offset);
    AssignValue("modf_offset", &modf_offset);
  }
};

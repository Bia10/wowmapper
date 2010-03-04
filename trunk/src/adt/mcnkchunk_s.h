#pragma once

#include "mcvtchunk_s.h"
#include "mcnrchunk_s.h"

/*! \brief MCNK chunk */
struct McnkChunk_s: Chunk_s {
  BitSet32_t flags;
  uint32_t index_x;
  uint32_t index_y;
  McvtChunk_s mcvt_chunk;
  McnrChunk_s mcnr_chunk;
  BitSet32_t holes;
  glm::vec3 position;


  McnkChunk_s() {
    // initialize chunk fields
    field_offsets_.insert(FieldOffset_t("flags",      DATA_OFFSET + 0x00));
    field_offsets_.insert(FieldOffset_t("index_x",    DATA_OFFSET + 0x04));
    field_offsets_.insert(FieldOffset_t("index_y",    DATA_OFFSET + 0x08));
    field_offsets_.insert(FieldOffset_t("mcvt_chunk", DATA_OFFSET + 0x14));
    field_offsets_.insert(FieldOffset_t("mcnr_chunk", DATA_OFFSET + 0x18));
    field_offsets_.insert(FieldOffset_t("holes",      DATA_OFFSET + 0x3c));
    field_offsets_.insert(FieldOffset_t("position",   DATA_OFFSET + 0x68));
  }

  virtual void Initialize() {
    // assign field values
    uint32_t flags_tmp;
    AssignValue("flags", &flags_tmp);
    flags = BitSet32_t(flags_tmp);

    AssignValue("index_y", &index_x);
    AssignValue("index_x", &index_y);

    uint32_t holes_tmp;
    AssignValue("holes", &holes_tmp);
    holes = BitSet32_t(holes_tmp);

    AssignValue("position", &position);

    // initialize sub chunks
    uint32_t offset;
    AssignValue("mcvt_chunk", &offset);
    GetChunkByOffset(offset, &mcvt_chunk);

    AssignValue("mcnr_chunk", &offset);
    GetChunkByOffset(offset, &mcnr_chunk);
  }
};

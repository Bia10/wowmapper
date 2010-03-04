#pragma once

#include "../chunkreader_c.h"
#include "mhdrchunk_s.h"
#include "mcinchunk_s.h"
#include "mmdxchunk_s.h"
#include "mmidchunk_s.h"
#include "mwmochunk_s.h"
#include "mwidchunk_s.h"
#include "mddfchunk_s.h"
#include "modfchunk_s.h"
#include "mcnkchunk_s.h"

typedef std::map<uint32_t, ModfChunk_s> WmoMap;

/*! \brief Used to parse ADT files, just supply the buffer. */
class Adt_c : public ChunkReader_c {
 public:
  Adt_c() {}
  Adt_c(uint8_t *buffer, int64_t size);

  void Initialize();

  const MhdrChunk_s& mhdr_chunk() const { return mhdr_chunk_; }
  const McinChunk_s& mcin_chunk() const { return mcin_chunk_; }
  const MmdxChunk_s& mmdx_chunk() const { return mmdx_chunk_; }
  const MmidChunk_s& mmid_chunk() const { return mmid_chunk_; }
  const MwmoChunk_s& mwmo_chunk() const { return mwmo_chunk_; }
  const MwidChunk_s& mwid_chunk() const { return mwid_chunk_; }
  const MddfChunk_s& mddf_chunk() const { return mddf_chunk_; }
  const ModfChunk_s& modf_chunk() const { return modf_chunk_; }

 private:
  void InitMcnkChunks();
  void InitWmoMap();

  MhdrChunk_s mhdr_chunk_;
  McinChunk_s mcin_chunk_;
  MmdxChunk_s mmdx_chunk_;
  MmidChunk_s mmid_chunk_;
  MwmoChunk_s mwmo_chunk_;
  MwidChunk_s mwid_chunk_;
  MddfChunk_s mddf_chunk_;
  ModfChunk_s modf_chunk_;
};

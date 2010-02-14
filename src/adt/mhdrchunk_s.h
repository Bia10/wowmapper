#pragma once

#include "mcinchunk_s.h"
#include "mwmochunk_s.h"
#include "modfchunk_s.h"

/*! @brief MHDR chunk */
struct MhdrChunk_s : Chunk_s {
  uint32_t flags;
  McinChunk_s *mcin;
  uint32_t mtex;
  uint32_t mmdx;
  uint32_t mmid;
  MwmoChunk_s *mwmo;
  uint32_t mwid;
  uint32_t mddf;
  ModfChunk_s *modf;
  uint32_t mfbo;
  uint32_t mh2o;
  uint32_t mtfx;

 private:
  uint32_t pad[4];

 public:
  MhdrChunk_s(uint32_t offset, void *buffer) : Chunk_s(offset, buffer, true) {
    /* intialize sub chunks: MCIN */
    uint32_t mcin_offset = reinterpret_cast<uint32_t>(mcin);
    mcin = new McinChunk_s(mcin_offset + 0x14, buffer);

    /* intialize sub chunks: MWMO */
    uint32_t mwmo_offset = reinterpret_cast<uint32_t>(mwmo);
    mwmo = new MwmoChunk_s(mwmo_offset + 0x14, buffer);

    /* intialize sub chunks: MODF */
    uint32_t modf_offset = reinterpret_cast<uint32_t>(modf);
    modf = new ModfChunk_s(modf_offset + 0x14, buffer);
  }

  ~MhdrChunk_s() {
    SAFE_DELETE(mcin);
    SAFE_DELETE(mwmo);
  }
};

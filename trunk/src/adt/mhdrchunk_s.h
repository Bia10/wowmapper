#pragma once

#include "mcinchunk_s.h"

/*! @brief MHDR chunk */
struct MhdrChunk_s : Chunk_s {
  uint32_t flags;
  McinChunk_s *mcin;
  uint32_t mtex;
  uint32_t mmdx;
  uint32_t mmid;
  uint32_t mwmo;
  uint32_t mwid;
  uint32_t mddf;
  uint32_t modf;
  uint32_t mfbo;
  uint32_t mh2o;
  uint32_t mtfx;

 private:
  uint32_t pad[4];

 public:
  MhdrChunk_s(int32_t memAbsOffset, void *in_buf) {
    int32_t buf_addr = reinterpret_cast<int32_t>(in_buf);
    void *dest_addr = reinterpret_cast<void*>(buf_addr + memAbsOffset);

    /* fill MHDR chunk with its real content */
    memcpy(this, dest_addr, sizeof(MhdrChunk_s));

    /* intialize sub chunks: MCIN */
    int32_t mcin_offset = reinterpret_cast<int32_t>(mcin);
    mcin = new McinChunk_s(mcin_offset + 0x14, in_buf);
  }

  ~MhdrChunk_s() {
    SAFE_DELETE(mcin);
  }
};

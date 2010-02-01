#pragma once

#include "mcinchunk_s.h"

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
};

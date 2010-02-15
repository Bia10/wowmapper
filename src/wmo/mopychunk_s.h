#pragma once

#include "../chunk_s.h"

/*! @brief MOPY chunk. */
struct MopyChunk_s : Chunk_s {
  struct PolyInfo_s {
    uint8_t flags;
    uint8_t material_id;
  };

  typedef std::vector<PolyInfo_s> PolyInfo_t;

  PolyInfo_t poly_info;

  MopyChunk_s(uint32_t offset, void *buffer) : Chunk_s(offset, buffer, false) {
    /* copy MOPY chunk manually */
    uint32_t buf_addr = reinterpret_cast<uint32_t>(buffer);
    void *src_addr = reinterpret_cast<void*>(buf_addr + offset + sizeof(Chunk_s));

    poly_info.resize(size / sizeof(PolyInfo_s));
    memcpy(poly_info.data(), src_addr, size);
  }
};

typedef std::auto_ptr<MopyChunk_s> MopyChunk_t;

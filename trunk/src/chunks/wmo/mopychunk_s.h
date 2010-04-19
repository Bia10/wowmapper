#pragma once

#include "../chunk_c.h"

/*! \brief MOPY: Map Object Material Info. */
struct MopyChunk_s : public Chunk_c {
  struct MaterialInfo_s {
    uint8_t flags;
    uint8_t id;
  };
  typedef std::vector<MaterialInfo_s> MaterialInfo_t;


  MaterialInfo_t mat_info;

  MopyChunk_s(Chunk_c *parent) : Chunk_c(parent) { }

 protected:
  virtual void LateInit() {
    mat_info.resize(buffer_.size()/sizeof(MaterialInfo_s));
    CopyDataBlock(buffer_, &mat_info);
  }
};

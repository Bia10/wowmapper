#pragma once

#include <stdint.h>
#include <vector>
#include "mhdrchunk_s.h"

struct vertex3f {
  float x, y ,z;

  vertex3f();
  vertex3f(float _x, float _y, float _z);
};

/*! @brief Used to parse ADT files, just supply the buffer.
 *  @remark Adt_c dtor will destroy the buffer. */
class Adt_c {
 public:
  Adt_c(uint8_t **buffer);
  virtual ~Adt_c();
  void GenerateMesh();

  const uint8_t* raw_data() const { return raw_data_; }
  const MhdrChunk_s& mhdr_chunk() const { return mhdr_chunk_; }
  const vertex3f* map_patches() const { return map_patches_; }
  const vertex3f* map_normals() const { return map_normals_; }

 private:
  uint8_t *raw_data_;
  MhdrChunk_s mhdr_chunk_;
  vertex3f map_patches_[3*4*64*256];
  vertex3f map_normals_[4*64*256];
};

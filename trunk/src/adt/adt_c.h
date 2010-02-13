#pragma once

#include <stdint.h>
#include <list>
#include "mhdrchunk_s.h"
#include "../mpqhandler_c.h"
#include "../wmo/wmo_c.h"

typedef std::list<Wmo_c> WmoList_t;

/*! @brief Used to parse ADT files, just supply the buffer.
 *  @remark Adt_c dtor will destroy the buffer. */
class Adt_c {
 public:
  Adt_c(uint8_t **buffer, MpqHandler_c *mpqHandler = NULL);
  virtual ~Adt_c();
  void GenerateMesh();

  const uint8_t* raw_data() const { return raw_data_; }
  const MhdrChunk_s& mhdr_chunk() const { return mhdr_chunk_; }
  const glm::vec3* vertices() const { return vertices_; }
  const glm::vec3* normals() const { return normals_; }

 private:
  uint8_t *raw_data_;
  MhdrChunk_s mhdr_chunk_;
  WmoList_t wmo_list_;
  glm::vec3 vertices_[3*4*64*256];
  glm::vec3 normals_[3*4*64*256];
};

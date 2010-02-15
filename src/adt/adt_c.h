#pragma once

#include <stdint.h>
#include <list>
#include <map>
#include "mhdrchunk_s.h"
#include "../mpqhandler_c.h"
#include "../wmo/wmo_c.h"

typedef std::list<Wmo_c*> WmoList_t;

/*! @brief Used to parse ADT files, just supply the buffer.
 *  @remark Adt_c dtor will destroy the buffer. */
class Adt_c {
 public:
  Adt_c(uint8_t **buffer, MpqHandler_c *mpqHandler = NULL);

  /* adt related */
  const MhdrChunk_s& mhdr_chunk() const { return mhdr_chunk_; }
  const glm::vec3* vertices() const { return vertices_; }
  const glm::vec3* normals() const { return normals_; }
  /* wmo related */
  const WmoList_t& wmo_list() const { return wmo_list_; }

 private:
  void LoadWmo(MpqHandler_c *mpqHandler);

  MhdrChunk_s mhdr_chunk_;
  WmoList_t wmo_list_;
  glm::vec3 vertices_[3*4*64*256];
  glm::vec3 normals_[3*4*64*256];
};

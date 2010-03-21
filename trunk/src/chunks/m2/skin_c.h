#pragma once

#include "../chunk_c.h"

class Skin_c : Chunk_c {
 public:
  Skin_c(const uint8_t *buffer, uint32_t length);

  void get_index_buffer(Indices16_t *buffer) const;

 private:
  Indices16_t indices_;
  Indices16_t triangles_;
};

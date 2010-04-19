#pragma once

#include "../chunk_c.h"

/*! \brief Skin provides indices for M2 models. */
class Skin_c : public Chunk_c {
 public:
  Skin_c(Buffer_t *buffer);

  void GetIndices(Indices32_t *buffer) const;

 private:
  Indices16_t indices_;
  Indices16_t triangles_;
};

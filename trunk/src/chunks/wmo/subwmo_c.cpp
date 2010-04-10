#include "subwmo_c.h"

SubWmo_c::SubWmo_c(Buffer_t *buffer)
    : Chunk_c(buffer), mogp_(this), mopy_(this), movi_(this),
      movt_(this), monr_(this) {
  GetSubChunk("PGOM", 4, &mogp_);
  GetSubChunk("YPOM", 4, &mopy_);
  GetSubChunk("IVOM", 4, &movi_);
  GetSubChunk("TVOM", 4, &movt_);
  GetSubChunk("RNOM", 4, &monr_);
}

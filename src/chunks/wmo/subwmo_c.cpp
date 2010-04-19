#include "subwmo_c.h"

SubWmo_c::SubWmo_c(Buffer_t *buffer)
    : Chunk_c(buffer), mogp_(this), mopy_(this), movi_(this),
      movt_(this), monr_(this) {
  GetSubChunk("PGOM", &mogp_);
  GetSubChunk("YPOM", &mopy_);
  GetSubChunk("IVOM", &movi_);
  GetSubChunk("TVOM", &movt_);
  GetSubChunk("RNOM", &monr_);
}

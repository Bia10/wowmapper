#include "subwmo_c.h"

SubWmo_c::SubWmo_c(Buffer_t *buffer)
    : Chunk_c(buffer),
      mogp_(this, 0x0c),
      mopy_(this, 0x58),
      movi_(this, mopy_.GetOffsetToNext()),
      movt_(this, movi_.GetOffsetToNext()),
      monr_(this, movt_.GetOffsetToNext()) {

}

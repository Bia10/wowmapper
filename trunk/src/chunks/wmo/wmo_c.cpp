#include "wmo_c.h"

Wmo_c::Wmo_c(const uint8_t *buffer, uint32_t length, MpqHandler_c &mpq_h)
    : Chunk_c(buffer, length),
      mohd_(this), mogn_(this), mogi_(this), mods_(this), modd_(this) {
  GetSubChunk("DHOM", 4, &mohd_);
  GetSubChunk("NGOM", 4, &mogn_);
  GetSubChunk("IGOM", 4, &mogi_);
  GetSubChunk("SDOM", 4, &mods_);
  GetSubChunk("DDOM", 4, &modd_);
}

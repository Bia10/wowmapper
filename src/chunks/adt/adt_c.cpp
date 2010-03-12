#include "adt_c.h"
#include "../m2/m2_c.h"

Adt_c::Adt_c(const uint8_t *buffer, uint32_t length, MpqHandler_c &mpq_h)
		: Chunk_c(buffer, length),
			mhdr_(this), mmdx_(this), mmid_(this), mwmo_(this),
			mwid_(this), mddf_(this), modf_(this), mh2o_(this) {
	GetSubChunk("RDHM", 4, &mhdr_);
	GetSubChunk("XDMM", 4, &mmdx_);
	GetSubChunk("DIMM", 4, &mmid_);
	GetSubChunk("OMWM", 4, &mwmo_);
	GetSubChunk("DIWM", 4, &mwid_);
	GetSubChunk("FDDM", 4, &mddf_);
	GetSubChunk("FDOM", 4, &modf_);
	GetSubChunk("O2HM", 4, &mh2o_);

	uint8_t *fbuf = NULL;

	if (mmdx_.m2_names.size()) {
	  std::cout << mmdx_.m2_names.c_str() << std::endl;
	  int64_t size = mpq_h.LoadFileByName(mmdx_.m2_names.c_str(), &fbuf);
	  M2_c(fbuf, size);
	}
}

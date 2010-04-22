#pragma once

#include "../chunk_c.h"

/*! \brief MHDR: Map Chunk Header. */
struct MhdrChunk_s : public Chunk_c {
  off_t mcin_off; //!< MCIN offset
  off_t mtex_off; //!< MTEX offset
  off_t mmdx_off; //!< MMDX offset
  off_t mmid_off; //!< MMID offset
  off_t mwmo_off; //!< MWMO offset
  off_t mwid_off; //!< MWID offset
  off_t mddf_off; //!< MDDF offset
  off_t modf_off; //!< MODF offset
  off_t mfbo_off; //!< MFBO offset
  off_t mh2o_off; //!< MH2O offset
  off_t mftx_off; //!< MFTX offset

  MhdrChunk_s(Chunk_c *parent, off_t off);

 private:
	const static off_t SUB_OFFSET = 0x14; // relative offset for sub chunks
};

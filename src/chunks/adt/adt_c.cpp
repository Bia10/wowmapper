#include "adt_c.h"
#include "../m2/m2_c.h"

Adt_c::Adt_c(const uint8_t *buffer, uint32_t length, MpqHandler_c &mpq_h)
		: Chunk_c(buffer, length),
			mhdr_(this), mmdx_(this), mmid_(this), mwmo_(this),
			mwid_(this), mddf_(this), modf_(this), mh2o_(this) {
	GetSubChunk("RDHM", 4, &mhdr_);  // header
	GetSubChunk("XDMM", 4, &mmdx_);  // model filenames
	GetSubChunk("DIMM", 4, &mmid_);  // model ids (offsets to filenames)
	GetSubChunk("OMWM", 4, &mwmo_);  // wmo filenames
	GetSubChunk("DIWM", 4, &mwid_);  // wmo ids (offsets to filenames)
	GetSubChunk("FDDM", 4, &mddf_);  // doodad information
	GetSubChunk("FDOM", 4, &modf_);  // wmo information
	GetSubChunk("O2HM", 4, &mh2o_);  // water information

	vertices_.reserve(256*145);
	normals_.reserve(256*145);
	indices_.reserve(256*768);

	for (McinChunk_s::McnkChunks_t::iterator mcnk = mhdr_.mcin.mcnks.begin();
	     mcnk != mhdr_.mcin.mcnks.end();
	     ++mcnk) {
	  Points_t vertices, normals;
	  Indices32_t indices;
	  mcnk->mcvt.GetVertices(&vertices);
	  mcnk->mcnr.GetNormals(&normals);
	  mcnk->mcvt.GetIndices(&indices);

	  vertices_.insert(vertices_.end(), vertices.begin(), vertices.end());
	  normals_.insert(normals_.end(), normals.begin(), normals.end());

	  // increment indices by size of vertices
	  Indices32_t add(768, vertices_.size());
	  std::transform(indices.begin(), indices.end(),
	      add.begin(), indices.begin(), std::plus<uint32_t>());

	  indices_.insert(indices_.end(), indices.begin(), indices.end());
	}
}

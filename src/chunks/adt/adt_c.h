#pragma once

#include "mhdrchunk_s.h"
#include "mmdxchunk_s.h"
#include "mmidchunk_s.h"
#include "mwmochunk_s.h"
#include "mwidchunk_s.h"
#include "mddfchunk_s.h"
#include "modfchunk_s.h"
#include "mh2ochunk_s.h"
#include "../m2/m2_c.h"
#include "../m2/skin_c.h"
#include "../../mpqhandler_c.h"

typedef std::map<std::string, M2_c*> DoodadMap_t;
typedef std::pair<std::string, M2_c*> DoodadPair_t;

/*! \brief ADT files. */
class Adt_c : public Chunk_c {
 public:
	Adt_c(const uint8_t *buffer, uint32_t length, MpqHandler_c &mpq_h);
	virtual ~Adt_c();

	const Points_t& vertices() const { return vertices_; }
	const Points_t& normals() const { return normals_; }
	const Indices32_t& indices() const { return indices_; }
	const Indices32_t& colors() const { return colors_; }

 private:
	void GenerateTerrain();
	void InsertDoodads(MpqHandler_c &mpq_h);
	void InsertVisibleTriangles(const M2_c &m2, const Skin_c &skin,
	                           const MddfChunk_s::DoodadInfo_s &info);
	void InsertBoundingTriangles(const M2_c &m2,
                               const MddfChunk_s::DoodadInfo_s &info);
	void TransformVertices(const glm::vec3 &translation, const glm::vec3 &rotation,
                         float scale, Points_t *vertices);

	MhdrChunk_s mhdr_; //!< header
	MmdxChunk_s mmdx_; //!< model filenames
	MmidChunk_s mmid_; //!< model ids (offsets to filenames)
	MwmoChunk_s mwmo_; //!< wmo filenames
	MwidChunk_s mwid_; //!< wmo ids (offsets to filenames)
	MddfChunk_s mddf_; //!< doodad information
	ModfChunk_s modf_; //!< wmo information
	Mh2oChunk_s mh2o_; //!< water information

	Points_t vertices_;
	Points_t normals_;
	Indices32_t indices_;
	Indices32_t colors_;
	DoodadMap_t doodads_;
};

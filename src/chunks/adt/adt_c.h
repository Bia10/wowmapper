#pragma once

#include "mhdrchunk_s.h"
#include "../m2/m2_c.h"
#include "../m2/skin_c.h"
#include "../wmo/wmo_c.h"
#include "../../mpqhandler_c.h"

struct M2Skin_s {
  M2_c *m2;
  Skin_c *skin;
};

typedef std::map<std::string, M2_c*> M2Map_t;
typedef std::pair<std::string, M2_c*> M2Pair_t;
typedef std::map<std::string, M2Skin_s> M2SkinMap_t;
typedef std::pair<std::string, M2Skin_s> M2SkinPair_t;

/*! \brief ADT files. */
class Adt_c : public Chunk_c {
 public:
	Adt_c(Buffer_t *buffer, MpqHandler_c &mpq_h);
	virtual ~Adt_c();

	const Points_t& vertices() const { return vertices_; }
	const Points_t& normals() const { return normals_; }
	const Indices32_t& indices() const { return indices_; }
	const Indices32_t& colors() const { return colors_; }

 private:
	void GenerateTerrain();
	void GenerateWater();
	void InsertDoodads(MpqHandler_c &mpq_h, bool useCollisionModel);
	void InsertVisibleTriangles(const M2_c &m2, const Skin_c &skin,
	                           const MddfChunk_s::DoodadInfo_s &info);
	void InsertBoundingTriangles(const M2_c &m2,
                               const MddfChunk_s::DoodadInfo_s &info);
	void TransformVertices(const glm::vec3 &translation, const glm::vec3 &rotation,
                         float scale, Points_t *vertices);

	MhdrChunk_s mhdr_; //!< header

	Points_t vertices_;
	Points_t normals_;
	Indices32_t indices_;
	Indices32_t colors_;

	M2Map_t m2_map_;
	M2SkinMap_t m2skin_map_;

	static Indices32_t obj_uids_;
};

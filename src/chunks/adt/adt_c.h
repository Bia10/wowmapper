#pragma once

#include "mhdrchunk_s.h"
#include "../m2/m2_c.h"
#include "../m2/skin_c.h"
#include "../wmo/wmo_c.h"
#include "../../mpqhandler_c.h"
#include "../../recast/Recast.h"
#include "../../recast/RecastLog.h"
#include "../../recast/RecastTimer.h"
#include "../../recast/ChunkyTriMesh.h"

/*struct Tile
{
  inline Tile() : chf(0), solid(0), cset(0), pmesh(0), dmesh(0), buildTime(0) {}
  inline ~Tile() { delete chf; delete cset; delete solid; delete pmesh; delete dmesh; }
  rcCompactHeightfield* chf;
  rcHeightfield* solid;
  rcContourSet* cset;
  rcPolyMesh* pmesh;
  rcPolyMeshDetail* dmesh;
  int buildTime;
};

struct TileSet
{
  inline TileSet() : width(0), height(0), tiles(0) {}
  inline ~TileSet() { delete [] tiles; }
  int width, height;
  float bmin[3], bmax[3];
  float cs, ch;
  Tile* tiles;
};*/

typedef std::map<std::string, M2_c*> M2Map_t;
typedef std::pair<std::string, M2_c*> M2Pair_t;

/*! \brief ADT files. */
class Adt_c : public Chunk_c {
 public:
	Adt_c(Buffer_t *buffer, MpqHandler_c &mpq_h);
	virtual ~Adt_c();

 private:
	/*! \brief Builds terrain geometry.
	 *  \param removeWet Removes terrain vertices covered by water. */
	void BuildTerrain(bool removeWet);
	/*! \brief Builds water geometry. */
	void BuildWater();
	/*! \brief Retrieve M2 models referenced by ADTs.
	 *  \param mpq_h MPQ handler to load M2 files from MPQs.
	 *  \param useCollisionModel True -> use collision model, false -> real model. */
	void GetDoodads(MpqHandler_c &mpq_h, bool useCollisionModel);
	/*! \brief Retrieve WMOs models referenced by ADTs.
	 *  \param mpq_h W handler to load WMO files from MPQs. */
	void GetWmos(MpqHandler_c &mpq_h);

	/*! \brief Checks if an UID already exists.
	 *  \param uid UID to check.
	 *  \return Returns true if UID already exists.
	 *  \remark If UID doesnt exist it gets added to the list of UIDs. */
	bool CheckUid(uint32_t uid) const;
	M2_c* GetM2(MpqHandler_c &mpq_h, const std::string &filename, bool loadSkin);

	MhdrChunk_s mhdr_;             //!< MHDR chunk
	Mesh_s terrain_;               //!< Terrain mesh
	Mesh_s water_;                 //!< Water mesh
	Meshes_t m2s_;                 //!< M2 meshes
	Meshes_t wmos_;                //!< WMO meshes
	M2Map_t m2_map_;               //!< M2 map to buffer already loaded M2s

	static Indices32_t obj_uids_;  //!< Unique object identifiers to load models only once
};

#pragma once

#include "mhdrchunk_s.h"
#include "../m2/m2_c.h"
#include "../m2/skin_c.h"
#include "../wmo/wmo_c.h"
#include "../../mpqhandler_c.h"

class Model_c;
typedef std::map<std::string, Model_c*> ModelMap_t;
typedef std::pair<std::string, Model_c*> ModelPair_t;

/*! \brief ADT files are the center for WoW terrain and models. It contains
 *         all information like terrain heightmap, water, wmos, m2, etc. */
class Adt_c : public Chunk_c {
 public:
	Adt_c(Buffer_t *buffer, MpqHandler_c &mpq_h);

	static void CleanUp();

	const Mesh_s& terrain() const { return terrain_; }
	const Meshes_t& m2s() const { return m2s_; }
	const Meshes_t& wmos() const { return wmos_; }
	// TODO: const Mesh_s& water() const { return water_; }

 private:
	/*! \brief Builds terrain geometry.
	 *  \param removeWet Removes terrain vertices covered by water. */
	void BuildTerrain(bool removeWet);
	/*! \brief Builds water geometry. */
	//void BuildWater();
	/*! \brief Retrieve M2 models referenced by ADTs.
	 *  \param mpq_h MPQ handler to load M2 files from MPQs.
	 *  \param useCollisionModel True -> use collision model, false -> real model. */
	void ParseDoodads(MpqHandler_c &mpq_h, bool useCollisionModel);
	/*! \brief Retrieve WMOs models referenced by ADTs.
	 *  \param mpq_h W handler to load WMO files from MPQs. */
	void ParseWmos(MpqHandler_c &mpq_h);
	/*! \brief Checks if an UID already exists.
	 *  \param uid UID to check.
	 *  \return Returns true if UID already exists.
	 *  \remark If UID doesnt exist it gets added to the list of UIDs. */
	bool CheckUid(uint32_t uid) const;
	/*! \brief Returns a M2 model from the buffer or loads it if not yet in buffer.
	 *  \param mpq_h MPQ handler.
	 *  \param filename M2 filename.
	 *  \param loadSkin True, load skin for m2. False, use bounding volume.
	 *  \return Returns a M2 model. */
	M2_c* GetM2(MpqHandler_c &mpq_h, const std::string &filename, bool loadSkin);
	/*! \brief Same behavior as GetM2.
	 *  \param mpq_h MPQ handler.
   *  \param filename M2 filename.
   *  \return Returns a WMO model. */
	Wmo_c* GetWmo(MpqHandler_c &mpq_h, const std::string &filename);

	MhdrChunk_s mhdr_;             //!< MHDR chunk
	Mesh_s terrain_;               //!< Terrain mesh
	Mesh_s water_;                 //!< Water mesh
	Meshes_t m2s_;                 //!< M2 meshes
	Meshes_t wmos_;                //!< WMO meshes

	static ModelMap_t model_map_;  //!< model map to buffer already loaded models
	static Indices32_t obj_uids_;  //!< Unique object identifiers to load models only once
};

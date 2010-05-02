#pragma once

#include "mhdrchunk_s.h"
#include "mcinchunk_s.h"
#include "mmdxchunk_s.h"
#include "mmidchunk_s.h"
#include "mwmochunk_s.h"
#include "mwidchunk_s.h"
#include "mddfchunk_s.h"
#include "modfchunk_s.h"
#include "mh2ochunk_s.h"
#include "mcnkchunk_s.h"
#include "../m2/m2_c.h"
#include "../m2/skin_c.h"
#include "../wmo/wmo_c.h"
#include "../../mpqhandler_c.h"
#include "../../mesh_c.h"

// mcnk chunks
typedef std::vector<McnkChunk_s*> McnkChunks_t;

// water chunk auto pointer
typedef std::auto_ptr<Mh2oChunk_s> Mh2oChunk_p;

// doodads
struct AdtDoodad_s {
  const M2_c *m2;
  const MddfChunk_s::DoodadInfo_s *info;
};
typedef std::list<AdtDoodad_s> AdtDoodads_t;

// wmos
struct Wmo_s {
  const Wmo_c *wmo;
  const ModfChunk_s::WmoInfo_s *info;
};
typedef std::list<Wmo_s> Wmos_t;

/*! \brief ADT files are the center for WoW terrain and models. It contains
 *         all information like terrain heightmap, water, wmos, m2, etc. */
class Adt_c : public Chunk_c {
 public:
	Adt_c(Buffer_t *buffer, Indices32_t *obj_uids);
	virtual ~Adt_c();

	static void CleanUp();

	/*! \brief Builds terrain geometry.
   *  \param removeWet Removes terrain vertices covered by water. */
  void BuildTerrain(bool removeWet, Mesh_c *mesh);
  /*! \brief Retrieve M2 models referenced by ADTs.
   *  \param mpq_h MPQ handler to load M2 files from MPQs.
   *  \param loadSkin False -> use collision model, true -> real model. */
  void LoadDoodads(MpqHandler_c &mpq_h, bool loadSkin);
  void LoadWmos(MpqHandler_c &mpq_h, bool loadSkin);

  void GetDoodads(Meshes_t *meshes);
  void GetWmos(Meshes_t *wmos, Meshes_t *doodads) const;


 private:
	/*! \brief Checks if an UID already exists.
	 *  \param uid UID to check.
	 *  \return Returns true if UID already exists.
	 *  \remark If UID doesnt exist it gets added to the list of UIDs. */
	bool UidAlreadyIn(uint32_t uid) const;
	/*! \brief Returns a M2 model from the buffer or loads it if not yet in buffer.
	 *  \param mpq_h MPQ handler.
	 *  \param filename M2 filename.
	 *  \param loadSkin True, load skin for m2. False, use bounding volume.
	 *  \return Returns a M2 model. */
	M2_c* GetM2(MpqHandler_c &mpq_h, const std::string &filename, bool loadSkin);
	/*! \brief Same behavior as GetM2.
	 *  \param mpq_h MPQ handler.
   *  \param filename M2 filename.
   *  \param loadSkin True, load skin for m2. False, use bounding volume.
   *  \return Returns a WMO model. */
	Wmo_c* GetWmo(MpqHandler_c &mpq_h, const std::string &filename, bool loadSkin);
	void InitMcnks();

	Indices32_t *obj_uids_;

	MhdrChunk_s mhdr_;                //!< MHDR chunk
	McinChunk_s mcin_;                //!< map chunks
  MmdxChunk_s mmdx_;                //!< model filenames
  MmidChunk_s mmid_;                //!< model ids (offsets to filenames)
  MwmoChunk_s mwmo_;                //!< wmo filenames
  MwidChunk_s mwid_;                //!< wmo ids (offsets to filenames)
  MddfChunk_s mddf_;                //!< doodad information
  ModfChunk_s modf_;                //!< wmo information
  Mh2oChunk_p mh2o_;                //!< water information

  AdtDoodads_t doodads_;
  Wmos_t wmos_;

  McnkChunks_t mcnks_;

	static ModelMap_t model_map_;  //!< model map to buffer already loaded models
};

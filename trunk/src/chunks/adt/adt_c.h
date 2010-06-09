/*  wowmapper - World of Warcraft MAP PARser
    Copyright (C) 2010 PferdOne

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>. */

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
  /*! \brief ADT constructor taking a buffer and an index vector to manage UIDs.
   *  \param buffer Buffer provided by an mpq handler.
   *  \param objUids Unique identifiers for adt wmos and doodads.
   *  \param modelMap Used as a buffer for already loaded models. */
	Adt_c(Buffer_t *buffer, Indices32_t *objUids, ModelMap_t *modelMap);
	virtual ~Adt_c();
	/*! \brief Builds terrain geometry.
   *  \param removeWet Removes terrain vertices covered by water.
   *  \param mesh Mesh where is put. */
  void BuildTerrain(bool removeWet, Mesh_c *mesh);
  /*! \brief Retrieve M2 models referenced by ADTs.
   *  \param mpqH MPQ handler to load M2 files from MPQs.
   *  \param loadSkin False -> use collision model, true -> real model. */
  void LoadDoodads(MpqHandler_c &mpqH, bool loadSkin);
  /*! \brief Tells the adt to load WMOs.
   *  \param mpqH MPQ handler to load WMO files from MPQs.
   *  \param loadSkin Here loadSkin refers to doodads inside wmos. */
  void LoadWmos(MpqHandler_c &mpqH, bool loadSkin);
  /*! \brief Returns terrain doodads as a vector of meshes. */
  void GetDoodads(Meshes_t *meshes);
  /*! \brief Returns terrain doodads as a vector of meshes. */
  void GetWmos(Meshes_t *wmos, Meshes_t *doodads) const;

 private:
  /*! \brief Initialize MCNKs. */
  void InitMcnks();
	/*! \brief Checks if an UID already exists.
	 *  \param uid UID to check.
	 *  \return Returns true if UID already exists.
	 *  \remark If UID doesnt exist it gets added to the list of UIDs. */
	bool UidAlreadyIn(uint32_t uid) const;
	/*! \brief Returns a M2 model from the buffer or loads it if not yet in buffer.
	 *  \param mpqH MPQ handler.
	 *  \param filename M2 filename.
	 *  \param loadSkin True, load skin for m2. False, use bounding volume.
	 *  \return Returns a M2 model. */
	M2_c* GetM2(MpqHandler_c &mpqH, const std::string &filename, bool loadSkin);
	/*! \brief Same behavior as GetM2.
	 *  \param mpqH MPQ handler.
   *  \param filename M2 filename.
   *  \param loadSkin True, load skin for m2. False, use bounding volume.
   *  \return Returns a WMO model. */
	Wmo_c* GetWmo(MpqHandler_c &mpqH, const std::string &filename, bool loadSkin);

	Indices32_t *obj_uids_;
	ModelMap_t *model_map_;  //!< model map to buffer already loaded models

	MhdrChunk_s mhdr_;       //!< MHDR chunk
	McinChunk_s mcin_;       //!< map chunks
  MmdxChunk_s mmdx_;       //!< model filenames
  MmidChunk_s mmid_;       //!< model ids (offsets to filenames)
  MwmoChunk_s mwmo_;       //!< wmo filenames
  MwidChunk_s mwid_;       //!< wmo ids (offsets to filenames)
  MddfChunk_s mddf_;       //!< doodad information
  ModfChunk_s modf_;       //!< wmo information
  Mh2oChunk_p mh2o_;       //!< water information

  AdtDoodads_t doodads_;
  Wmos_t wmos_;

  McnkChunks_t mcnks_;
};

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

#include "mohdchunk_s.h"
#include "motxchunk_s.h"
#include "momtchunk_s.h"
#include "mognchunk_s.h"
#include "mogichunk_s.h"
#include "mosbchunk_s.h"
#include "mopvchunk_s.h"
#include "moptchunk_s.h"
#include "moprchunk_s.h"
#include "movvchunk_s.h"
#include "movbchunk_s.h"
#include "moltchunk_s.h"
#include "modschunk_s.h"
#include "modnchunk_s.h"
#include "moddchunk_s.h"
#include "subwmo_c.h"
#include "../model_c.h"
#include "../m2/m2_c.h"
#include "../../mpqhandler_c.h"

typedef std::list<SubWmo_c*> SubWmos_t;

// doodads
struct WmoDoodad_s {
  const M2_c *m2;
  const ModdChunk_s::DoodadInfo_s *info;
};
typedef std::list<WmoDoodad_s> WmoDoodads_t;


class Wmo_c : public Model_c {
 public:
  Wmo_c(Buffer_t *buffer, const std::string &wmo_name, ModelMap_t *mm);
  virtual ~Wmo_c();

  //void ParseDoodads(MpqHandler_c &mpq_h);
  void LoadWmo(MpqHandler_c &mpq_h);
  void GetWmo(Mesh_c *mesh) const;
  void LoadDoodads(MpqHandler_c &mpq_h, bool loadSkin);
  void GetDoodads(Meshes_t *meshes, const Vec3_t &pos, const Vec3_t &rot) const;

 private:
  M2_c* GetM2(MpqHandler_c &mpq_h, const std::string &filename, bool loadSkin);

  std::string wmo_name_;
  ModelMap_t &model_map_;

  MohdChunk_s mohd_;
  MotxChunk_s motx_;  //!< TODO: not yet implemented
  MomtChunk_s momt_;  //!< TODO: not yet implemented
  MognChunk_s mogn_;
  MogiChunk_s mogi_;
  MosbChunk_s mosb_;  //!< Data is always 4 bytes of 0
  MopvChunk_s mopv_;  //!< TODO: not yet implemented
  MoptChunk_s mopt_;  //!< TODO: not yet implemented
  MoprChunk_s mopr_;  //!< TODO: not yet implemented
  MovvChunk_s movv_;  //!< TODO: not yet implemented
  MovbChunk_s movb_;  //!< TODO: not yet implemented
  MoltChunk_s molt_;  //!< TODO: not yet implemented
  ModsChunk_s mods_;
  ModnChunk_s modn_;
  ModdChunk_s modd_;

  SubWmos_t sub_wmos_;
  WmoDoodads_t doodads_;
};

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

#include "mogpchunk_s.h"
#include "mopychunk_s.h"
#include "movichunk_s.h"
#include "movtchunk_s.h"
#include "monrchunk_s.h"

/*! \brief Beside root WMOs there are group WMOs/sub WMOs, which contain the
 *         actual geometry data for the entire WMO. WMOs can consist of several
 *         group/sub WMOs. Best example is Stormwind. */
class SubWmo_c : public Chunk_c {
 public:
  SubWmo_c(Buffer_t *buffer);

  const Indices16_t& indices() const { return movi_.indices; }
  const Vertices_t& vertices() const { return movt_.vertices; }
  const Vertices_t& normals() const { return monr_.normals; }

  const MopyChunk_s& mopy() const { return mopy_; }

 private:
  MogpChunk_s mogp_;
  MopyChunk_s mopy_;
  MoviChunk_s movi_;
  MovtChunk_s movt_;
  MonrChunk_s monr_;
};

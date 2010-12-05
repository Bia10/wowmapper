/*  wowmapper - World of Warcraft MAP ParsER
    Copyright (C) 2010 PferdOne aka Flowerew

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

#include "common.h"
#include "mverchunk.h"

//------------------------------------------------------------------------------
struct MogpChunk_s {
  char id[4];
  uint32_t size;
  uint32_t nameIndex;
  uint32_t descriptionIndex;
  uint32_t flags;
  glm::vec3 bboxMin, bboxMax;
  uint16_t moprIndex;
  uint16_t numMoprItems;
  uint16_t numBatchesA;
  uint16_t numBatchesB;
  uint16_t numBatchesC;
  uint8_t fogIndices[4];
  uint32_t liquid;
  uint32_t wmoGroupId;
 private:
  uint32_t unknown[2];
};

//------------------------------------------------------------------------------
struct MopyChunk_s {
  struct MaterialInformation_s {
    uint8_t flags;
    uint8_t id;
  };
  typedef std::vector<MaterialInformation_s> MaterialInformations_t;

  char id[4];
  uint32_t size;
  MaterialInformations_t infos;
};

//------------------------------------------------------------------------------
struct MoviChunk_s {
  char id[4];
  uint32_t size;
  Indices16_t indices;
};

//------------------------------------------------------------------------------
struct MovtChunk_s {
  char id[4];
  uint32_t size;
  Vertices_t vertices;
};

//------------------------------------------------------------------------------
struct MonrChunk_s {
  char id[4];
  uint32_t size;
  Normals_t normals;
};

//------------------------------------------------------------------------------
struct MotvChunk_s {
  char id[4];
  uint32_t size;
  TexCoords_t texCoords;
};

//------------------------------------------------------------------------------
/** Parses WMO group files which contain actual geometry information for WMOs. **/
class WmoGroup {
 public:
  WmoGroup( BufferS_t &grp_buf );
  const MopyChunk_s& getMopyChunk() const;
  const MoviChunk_s& getMoviChunk() const;
  const MovtChunk_s& getMovtChunk() const;
  const MonrChunk_s& getMonrChunk() const;
  const MotvChunk_s& getMotvChunk() const;

 private:
   MverChunk_s _mverChunk;
   MogpChunk_s _mogpChunk;
   MopyChunk_s _mopyChunk;
   MoviChunk_s _moviChunk;
   MovtChunk_s _movtChunk;
   MonrChunk_s _monrChunk;
   MotvChunk_s _motvChunk;
};
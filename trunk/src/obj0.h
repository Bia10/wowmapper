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
/** MMDX chunk contains all model/doodad names used in an ADT. **/
struct MmdxChunk_s {
  char id[4];
  uint32_t size;
};
typedef std::vector<std::string> Names_t;

/** MMDX chunk contains all model/doodad names used in an ADT. **/
struct MwmoChunk_s {
  char id[4];
  uint32_t size;
};

/** MWID/MMID chunk is an index array to the names of the MWMO/MMDX chunk. If a
    WMO/doodad is referenced it will reference the index inside the MWID/MMID chunk
    which itself will point to the offset for the beginning of the string in
    the MWMO/MDDX chunk. **/
struct MmidChunk_s {
  char id[4];
  uint32_t size;
};

struct MwidChunk_s {
  char id[4];
  uint32_t size;
};
typedef std::vector<uint32_t> Offsets_t;
typedef std::vector<std::string> Names_t;

//------------------------------------------------------------------------------
/** MMDF holds positioning information for referenced doodads. **/
struct MddfChunk_s {
  char id[4];
  uint32_t size;

  /** Contains positining information aswell as as unique ID and the offset into
      the MMID index array. **/
  struct DoodadInfo_s {
    uint32_t id;
    uint32_t uid;
    glm::vec3 pos;
    glm::vec3 rot;
    uint16_t scale;
    uint16_t flags;
  };
};
typedef std::vector<MddfChunk_s::DoodadInfo_s> DoodadInformations_t;

//------------------------------------------------------------------------------
/** MODF holds positioning information for referenced WMOs. **/
struct ModfChunk_s {
  char id[4];
  uint32_t size;

  /** Contains positining information aswell as as unique ID and the offset into
      the MWID index array. **/
  struct WmoInfo_s {
    uint32_t id;
    uint32_t uid;
    glm::vec3 pos;
    glm::vec3 rot;
    glm::vec3 bbmin;
    glm::vec3 bbmax;
    uint32_t flags;
    uint16_t doodadSet;
    uint16_t nameSet;
  };
};
typedef std::vector<ModfChunk_s::WmoInfo_s> WmoInformations_t;

//------------------------------------------------------------------------------
/** MCNK chunk for _obj0 files. **/
struct ObjMcnkChunk_s {
  char id[4];
  uint32_t size;
};

/** MCRD chunk with doodad references per MCNK. **/
struct McrdChunk_s {
  char id[4];
  uint32_t size;
};

/** MCRW chunk with wmo references per MCNK. **/
struct McrwChunk_s {
  char id[4];
  uint32_t size;
};

/** Object references is a per MCNK struct which tells us what objects (doodad/WMO)
    is referenced by an MCNK. **/
struct ObjectReference_s {
  ObjMcnkChunk_s mcnkChunk;
  McrdChunk_s mcrdChunk;
  McrwChunk_s mcrwChunk;
  Indices32_t doodadIndices;
  Indices32_t wmoIndices;
};

typedef std::vector<ObjectReference_s> ObjectReferences_t;

/** All we need to know about a doodad. **/
struct Doodad_s {
  std::string name;
  MddfChunk_s::DoodadInfo_s info;
};

/** All we need to know about a WMO. **/
struct Wmo_s {
  std::string name;
  ModfChunk_s::WmoInfo_s info;
};


//------------------------------------------------------------------------------
class Obj0 {
 public:
  Obj0( const BufferS_t &obj_buf );

  void getDoodad( uint32_t index, Doodad_s *doodad ) const;
  void getWmo( uint32_t index, Wmo_s *wmo ) const;
  const ObjectReferences_t& getObjectRefs() const;
  const WmoInformations_t& wmoInfo() const;

 private:
  void parseObjectReferences( std::istream &i_str );

  MverChunk_s _mverChunk;
  MmdxChunk_s _mmdxChunk;
  MmidChunk_s _mmidChunk;
  MwmoChunk_s _mwmoChunk;
  MwidChunk_s _mwidChunk;
  MddfChunk_s _mddfChunk;
  MddfChunk_s _modfChunk;
  ObjectReferences_t _objectRefs;
  Offsets_t _mmdxIndices;
  Offsets_t _mwmoIndices;
  std::string _doodadNames;
  std::string _wmoNames;
  DoodadInformations_t _doodadInfo;
  WmoInformations_t _wmoInfo;
};

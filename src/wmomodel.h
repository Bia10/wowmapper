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
#include "mpqhandler.h"
#include "wmogroup.h"

//------------------------------------------------------------------------------
/** WMO header information chunk. **/
struct MohdChunk_s {
  char id[4];
  uint32_t size;
  uint32_t numMaterials;
  uint32_t numGroups;
  uint32_t numPortals;
  uint32_t numLights;
  uint32_t numModels;
  uint32_t numDoodads;
  uint32_t numSets;
  uint32_t ambientColor;
  uint32_t wmoId;
  glm::vec3 bboxMin, bboxMax;
  uint32_t liquidType;
};

//------------------------------------------------------------------------------
/** Texture names chunk. **/
struct MotxChunk_s {
  char id[4];
  uint32_t size;
  std::string textureNames;
};

//------------------------------------------------------------------------------
/** MOMT contains material informations. **/
struct MomtChunk_s {
  struct Materials_s {
    uint32_t flags;
    uint32_t specularMode;
    uint32_t blendMode;
    uint32_t texture0;
    uint32_t color0;
    uint32_t flags0;
    uint32_t texture1;
    uint32_t color1;
    uint32_t flags1;
    uint32_t color2;
   private:
    uint32_t pad[6];
  };
  typedef std::vector<Materials_s> Materials_t;

  char id[4];
  uint32_t size;
  Materials_t materials;
};


//------------------------------------------------------------------------------
/** Group names, not really used. **/
struct MognChunk_s {
  char id[4];
  uint32_t size;
  std::string groupNames;
};

//------------------------------------------------------------------------------
/** Group information chunks, again not really used. **/
struct MogiChunk_s {
  struct GroupInformation_s {
    uint32_t flags;
    glm::vec3 bboxMin, bboxMax;
    int32_t nameOffset;
  };
  typedef std::vector<GroupInformation_s> GroupInformations_t;

  char id[4];
  uint32_t size;
  GroupInformations_t infos;
};


//------------------------------------------------------------------------------
/** Sky box chunk. **/
struct MosbChunk_s {
  char id[4];
  uint32_t size;
  uint32_t unknown;
};

//------------------------------------------------------------------------------
/** Portal verices. **/
struct MopvChunk_s {
  struct PortalVertices_s {
    glm::vec3 vertex[4];
  };
  typedef std::vector<PortalVertices_s> PortalVertices_t;

  char id[4];
  uint32_t size;
  PortalVertices_t vertices;
};


//------------------------------------------------------------------------------
/** Portal geometry informatin. **/
struct MoptChunk_s {
  struct PortalInformation_s {
    uint16_t startVertex;
    uint16_t numVertices;
    glm::vec3 normal;
    uint32_t unknown;
  };
  typedef std::vector<PortalInformation_s> PortalInformations_t;

  char id[4];
  uint32_t size;
  PortalInformations_t infos;
};


//------------------------------------------------------------------------------
/** Portal references. **/
struct MoprChunk_s {
  struct PortalReference_s {
    uint16_t index;
    uint16_t groupIndex;
    uint16_t side;
    uint16_t pad;
  };
  typedef std::vector<PortalReference_s> PortalReferences_t;

  char id[4];
  uint32_t size;
  PortalReferences_t references;
};

//------------------------------------------------------------------------------
struct MovvChunk_s {
  char id[4];
  uint32_t size;
  std::vector<char> content;
};

//------------------------------------------------------------------------------
struct MovbChunk_s {
  char id[4];
  uint32_t size;
  uint16_t firstVertex;
  uint16_t count;
};

//------------------------------------------------------------------------------
/** Lighting information. **/
struct MoltChunk_s {
  struct LightInformation_s {
    uint8_t lightType;
    uint8_t type;
    uint8_t useAttenuation;
    uint8_t pad;
    uint32_t color;
    glm::vec3 position;
    float intensity;
    float attenuationStart;
    float attenuationEnd;
   private:
    float unknown[4];
  };
  typedef std::vector<LightInformation_s> LightInformations_t;

  char id[4];
  uint32_t size;
  LightInformations_t infos;
};

//------------------------------------------------------------------------------
/** Doodad sets. **/
struct ModsChunk_s {
  struct DoodadSet_s {
    char name[20];
    uint32_t firstIndex;
    uint32_t numDoodads;
   private:
    uint32_t unknown;
  };
  typedef std::vector<DoodadSet_s> DoodadSets_t;

  char id[4];
  uint32_t size;
  DoodadSets_t doodadSets;  
};

//------------------------------------------------------------------------------
/** Doodad names. **/
struct ModnChunk_s {
  char id[4];
  uint32_t size;
  std::string doodadNames; 
};

//------------------------------------------------------------------------------
/** Doodad information/definition. **/
struct ModdChunk_s {
  struct DoodadInformation_s {
    uint32_t id;
    glm::vec3 position;
    glm::vec4 rotation;
    float scale;
    uint32_t color;
  };
  typedef std::vector<DoodadInformation_s> DoodadInformations_t;

  char id[4];
  uint32_t size;
  DoodadInformations_t infos;
};

//------------------------------------------------------------------------------
//struct MfogChunk_s { };
//struct McvpChunk_s { };

typedef std::vector<WmoGroup*> WmoGroups_t;

//------------------------------------------------------------------------------
class WmoModel {
 public:
  WmoModel( const BufferS_t &wmo_buf );
  ~WmoModel();
  void loadGroups( const std::string wmo_name, MpqHandler &mpq_h );
  void getIndices( Indices32_t *indices, uint32_t filter = 0xff, uint32_t off = 0 ) const;
  void getVertices( Vertices_t *vertices ) const;
  void getNormals( Normals_t *normals ) const;

 private:
  MverChunk_s _mverChunk;
  MohdChunk_s _mohdChunk;
  MotxChunk_s _motxChunk;
  MomtChunk_s _momtChunk;
  MognChunk_s _mognChunk;
  MogiChunk_s _mogiChunk;
  MosbChunk_s _mosbChunk;
  MopvChunk_s _mopvChunk;
  MoptChunk_s _moptChunk;
  MoprChunk_s _moprChunk;
  MovvChunk_s _movvChunk;
  MovbChunk_s _movbChunk;
  MoltChunk_s _moltChunk;
  ModsChunk_s _modsChunk;
  ModnChunk_s _modnChunk;
  ModdChunk_s _moddChunk;
  WmoGroups_t _wmoGroups;
};
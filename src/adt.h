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
/** Was formely responsible to point to all chunks inside an ADT. This has changed
    with Cataclysm or WoW v4.0, and now only contains the offset to the MH2O chunk,
    though the size of the chunk hasn't changed at all, just the offset are 'gone'. **/
struct MhdrChunk_s {
  char id[4];
  uint32_t size;
  uint32_t flags;     // &1: MFBO, &2: unknown. in some Northrend ones.
  uint32_t mcinOff;   //!< MCIN offset
  uint32_t mtexOff;   //!< MTEX offset
  uint32_t mmdxOff;   //!< MMDX offset
  uint32_t mmidOff;   //!< MMID offset
  uint32_t mwmoOff;   //!< MWMO offset
  uint32_t mwidOff;   //!< MWID offset
  uint32_t mddfOff;   //!< MDDF offset
  uint32_t modfOff;   //!< MODF offset
  uint32_t mfboOff;   //!< MFBO offset
  uint32_t mh2oOff;   //!< MH2O offset
  uint32_t mftxOff;   //!< MFTX offset
 private:
  uint32_t pad[4];
};


//------------------------------------------------------------------------------
/** The MH20 chunk contains 256 header entries. The header points to
    the information blocks in the buffer. **/
struct Mh2oChunk_s {
  /** Headers contain offsets to the information blocks and to the
      render bitmap aswell. **/
  struct Header_s {
    /** Actual information about the water. **/
    struct Information_s {
      uint16_t type;
      uint16_t flags;
      float heightLevel1;
      float heightLevel2;
      uint8_t xOffset;
      uint8_t yOffset;
      uint8_t width;
      uint8_t height;
      Indices8_t *mask2;
      HeightMap_t *heightMap;
    };
    typedef std::vector<Information_s> Informations_t;

    Informations_t *infos;
    uint32_t numLayers;
    uint64_t *renderMap;
  };
  typedef std::vector<Header_s> Headers_t;

  char id[4];
  uint32_t size;
  Headers_t headers;  
};




//------------------------------------------------------------------------------
/** MCNK chunk for parsing terrain data. **/
struct McnkChunk_s {
  char id[4];
  uint32_t size;
  uint32_t flags;
  uint32_t indexX;
  uint32_t indexY;
  uint32_t numLayers;
  uint32_t numDoodads;
  uint32_t mcvtOff;
  uint32_t mcnrOff;
  uint32_t mclyOff;
  uint32_t mcrfOff;
  uint32_t mcalOff;
  uint32_t mcalSize;
  uint32_t mcshOff;
  uint32_t mcshSize;
  uint32_t areaId;
  uint32_t numWmos;
  uint32_t holes;
  uint64_t texmap0;
  uint64_t texmap1;
  uint32_t predTex;
  uint32_t noEffectDoodad;
  uint32_t msceOff;
  uint32_t numSoundEmitters;
  uint32_t mclqOff;
  uint32_t mclqSize;
  glm::vec3 position;
  uint32_t mccvOff;
 private:
  uint32_t pad[2];
};
typedef std::vector<McnkChunk_s> McnkChunks_t;


//------------------------------------------------------------------------------
/** MCVT chunk contains the heigt values of the height map. **/
struct McvtChunk_s {
  char id[4];
  uint32_t size;
};

//------------------------------------------------------------------------------
/** MCNR chunk contains normals for the height map. **/
struct McnrChunk_s {
  char id[4];
  uint32_t size;
};

//------------------------------------------------------------------------------
/** Terrain_s contains the terrain of an ADT tile. **/
struct Terrain_s {
  Indices32_t indices;
  Vertices_t vertices;
  Normals_t normals;
  HeightMap_t heightMap;
  glm::vec3 position;
  uint32_t areaId;

  static Indices32_t generatedIndices;
  static Vertices_t generatedVertices;
};
typedef std::vector<Terrain_s> AdtTerrain_t;


//------------------------------------------------------------------------------
class Adt {
 public:
  Adt( const BufferS_t &adt_buf );
  ~Adt();
  const AdtTerrain_t& getTerrain() const;

 private:
  /** Parses the MH2O chunk. **/
  bool parseMh2oChunk( std::istream &i_str );
  /** Parses the MCNK chunk. **/
  void parseMcnkChunk( std::istream &i_str, McnkChunk_s *mcnk_chunk,
                       Terrain_s *terrain );
  void getGeometry( Terrain_s *terrain ) const;
  /** This function is called before parsing any ADTs. It will generate
      the indices needed to construct the terrain from the heights. **/
  void generateIndices();
  /** This function is called before parsing any ADTs. It is similar to
      to generateIndices() but for vertices. All vertex positions are
      pre-generated except for the height value. **/
  void generateVertices();
  /** Marks holes by setting their indices to max unsigned int (-1). **/
  void markHoles( uint32_t holes, Terrain_s *terrain ) const;
  /** Removes geometry from buffers in terrain, which were marked with -1. **/
  void removeGeometry( Terrain_s *terrain ) const;

  MverChunk_s _mverChunk;
  MhdrChunk_s _mhdrChunk;
  Mh2oChunk_s _mh2oChunk;
  McnkChunks_t _mcnkChunks;
  AdtTerrain_t _adtTerrain;
};

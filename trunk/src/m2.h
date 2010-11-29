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
#include "obj0.h"

#pragma pack( push, 1 )
struct M2Header_s {
  char id[4];
  char version[4];
  uint32_t nameLength;
  uint32_t nameOff;
  uint32_t flags;
 private:
  uint32_t pad0[10];
 public:
  uint32_t numVertices;
  uint32_t verticesOff;
 private:
  uint32_t pad1[37];
 public:
  uint32_t numBoundingTriangles;
  uint32_t boundingTriangleOff;
  uint32_t numBoundingVertices;
  uint32_t boundingVerticesOff;
  uint32_t numBoundingNormals;
  uint32_t boundingNormalsOff;
};
#pragma pack( pop )


struct M2Data_s {
  glm::vec3 position;
  glm::vec3 normal;
  uint8_t boneWeight[4];
  uint8_t boneIndices[4];
  glm::vec2 texCoord;
  glm::vec2 unknown;
};

class M2 {
 public:
  M2( const BufferS_t &m2_buf );
  void getBoundingVertices( Vertices_t *vertices );
  void getBoundingIndices( Indices32_t *indices );
  void getBoundingNormals( Normals_t *normals );

 private:
  M2Header_s _m2Header;
  std::string _m2name;
  Vertices_t _vertices;
  Indices16_t _boundingIndices;
  Vertices_t _boundingVertices;
  Vertices_t _boundingNormals;
};
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

#include <map>
#include <vector>
#include <list>
#include <algorithm>
#include <string>
#include <iostream>
#include <exception>
#include <memory>
#include <iterator>
#include <sstream>
#include <fstream>
#include <float.h>
#include <limits>

#include <sys/types.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#ifndef WIN32
  #include <stdint.h>
  #include <dirent.h>
#else
  typedef signed char       int8_t;
  typedef unsigned char     uint8_t;
  typedef signed short      int16_t;
  typedef unsigned short    uint16_t;
  typedef signed int        int32_t;
  typedef unsigned int      uint32_t;
  typedef signed __int64    int64_t;
  typedef unsigned __int64  uint64_t;
#endif

typedef std::string BufferS_t;

typedef std::vector<glm::vec3> Vertices_t;
typedef std::vector<glm::vec2> TexCoords_t;
typedef std::vector<uint32_t> Indices32_t;
typedef std::vector<uint16_t> Indices16_t;
typedef std::vector<uint8_t> Indices8_t;
typedef std::vector<glm::vec3> Normals_t;
typedef std::vector<float> HeightMap_t;
typedef std::map<uint32_t, void*> UidMap_t;

static const size_t CHUNK_DATA = 0x8;


static void transformVertices( const glm::vec3 &pos, const glm::vec3 &rot, float scale,
                               Vertices_t *vertices ) {
  static const float map_offset = 17066.0f + (2/3.0f);
  static const glm::vec3 map_pos( map_offset, 0, map_offset );

  // create rotation matrix
  glm::mat4 rot_mtx;
  rot_mtx = glm::rotate( rot_mtx, rot.y - 90, glm::vec3( 0, 1, 0 ) );
  rot_mtx = glm::rotate( rot_mtx, -rot.x,     glm::vec3( 0, 0, 1 ) );
  rot_mtx = glm::rotate( rot_mtx, rot.z - 90, glm::vec3( 1, 0, 0 ) );

  // transform vertices
  for ( Vertices_t::iterator vertex = vertices->begin();
        vertex != vertices->end();
        ++vertex ) {
    glm::vec4 v4( *vertex, 0 );
    v4 = scale * rot_mtx * v4;
    *vertex = glm::vec3( v4 );
    *vertex += pos - map_pos;
  }
}

//------------------------------------------------------------------------------
static void mergeVertices( const Vertices_t &src, Vertices_t *dest ) {
  dest->insert( dest->end(), src.begin(), src.end() );
}

//------------------------------------------------------------------------------
static void mergeNormals( const Normals_t &src, Normals_t *dest ) {
  dest->insert( dest->end(), src.begin(), src.end() );
}

//------------------------------------------------------------------------------
static void mergeIndices( const Indices32_t &src, uint32_t off, Indices32_t *dest ) {
  Indices32_t increment( src.begin(), src.end() );

  // increment indices
  std::vector<uint32_t> add( src.size(), off );
  std::transform( increment.begin(), increment.end(), add.begin(),
                  increment.begin(), std::plus<uint32_t>() );
  // add new indices to destination
  dest->insert( dest->end(), increment.begin(), increment.end() );
}

static void quitApp( const std::string &msg ) {
  std::cout << msg << std::endl;
  std::cout << "quit" << std::endl;
  exit( 1 );
}

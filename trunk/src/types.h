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

// geometry related typedefs
typedef glm::vec2 Vec2_t;
typedef glm::vec3 Vec3_t;

typedef std::vector<Vec2_t>       TexCoords_t;
typedef std::vector<Vec3_t>       Vertices_t;
typedef std::vector<Vec3_t>       Normals_t;
typedef std::vector<uint32_t>     Colors_t;

typedef std::vector<uint8_t>      Buffer_t;
typedef std::vector<uint16_t>     Indices16_t;
typedef std::vector<uint32_t>     Indices32_t;
typedef std::vector<uint64_t>     Indices64_t;
typedef std::vector<std::string>  Strings_t;

typedef uint32_t wm_off_t;
typedef uint32_t wm_size_t;

struct BBox_s {
  Vec3_t min;
  Vec3_t max;

  BBox_s() : min(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()),
             max(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min()) {}
};

struct AdtPos_s {
  int x, y;
};
typedef std::list<AdtPos_s> AdtPos_t;

// file header struct
struct Header_s {
  char id[4];
  wm_size_t size;
};

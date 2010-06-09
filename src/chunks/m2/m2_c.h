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

#include "skin_c.h"
#include "../model_c.h"
#include "../../mesh_c.h"

struct M2Geometry_s {
  glm::vec3 position;
  glm::vec3 normal;
  uint8_t bone_weight[4];
  uint8_t bone_indices[4];
  glm::vec2 texcoord;
  glm::vec2 unknown;
};

typedef std::vector<M2Geometry_s> M2Geometry_t;
typedef std::auto_ptr<Skin_c> Skin_p;

/*! \brief M2 file handler. */
class M2_c : public Model_c {
 public:
  M2_c(Buffer_t *buffer);

  void GetMesh(const Skin_c &skin, Mesh_c *mesh) const;
  void GetVertices(Vertices_t *vertices) const;
  void GetNormals(Vertices_t *normals) const;
  /*! \brief Gets bounding volume mesh for you.
   *  \param mesh Mesh where bv mesh will be put.
   *  \remark Will resize mesh containers to make the content fit. */
  void GetBVMesh(Mesh_c *mesh) const;
  void GetBVIndices(Indices32_t *indices) const;
  void GetBVVertices(Vertices_t *vertices) const;
  void GetBVNormals(Vertices_t *normals) const;

  Skin_p skin;

 private:
  M2Geometry_t content_;
};

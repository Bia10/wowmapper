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

#include "common.h"

/*! \brief Mesh types. */
enum MeshType {
  MESH_TYPE_TERRAIN   = 0x1,
  MESH_TYPE_DOODADS   = 0x2,
  MESH_TYPE_WMOS      = 0x4,
  MESH_TYPE_UNDEFINED = 0xff,
};

class Mesh_c;
typedef std::vector<Mesh_c> Meshes_t;

/*! \brief Mesh class. */
class Mesh_c {
 public:
  /*! \brief Every vector you pass will be swapped! */
  Mesh_c();
  virtual ~Mesh_c();

  void CalcBoundingBox();

  // setter
  void SetName(const std::string &name);
  void SetType(MeshType meshType);
  void SetTransformation(const Vec3_t &pos, const Vec3_t &rot, float scale = 1.0f);
  void SetPositon(const Vec3_t &position);
  void SetRotation(const glm::quat &quaternion);
  void SetRotation(const Vec3_t &rotation);
  void SetScale(float scale);
  void SetGeometry(Indices32_t *idx, Vertices_t *vtx, Normals_t *norm);
  void SetIndices(Indices32_t *indices);
  void SetVertices(Vertices_t *vertices);
  void SetNormals(Normals_t *normals);
  void SetColors(Colors_t *colors);
  void SetBoundingBox(const BBox_s &bbox);

  // getter
  const std::string& name() const { return name_; }
  uint32_t type() const { return type_; }
  const Vec3_t& position() const { return position_; }
  const glm::quat& quaternion() const { return quaternion_; }
  const Vec3_t& rotation() const { return rotation_; }
  float scale() const { return scale_; }
  const Indices32_t& indices() const { return indices_; }
  const Vertices_t& vertices() const { return vertices_; }
  const Normals_t& normals() const { return normals_; }
  const TexCoords_t& texCoords() const { return tex_coords_; }
  const Colors_t& colors() const { return colors_; }
  const BBox_s& bbox() const { return bbox_; }

 private:
  // mesh identifiers
  std::string name_;
  uint32_t type_;

  // transformation
  Vec3_t position_;
  glm::quat quaternion_;
  Vec3_t rotation_;
  float scale_;

  // geometry data
  Indices32_t indices_;
  Vertices_t vertices_;
  Normals_t normals_;
  TexCoords_t tex_coords_;
  Colors_t colors_;
  BBox_s bbox_;
};

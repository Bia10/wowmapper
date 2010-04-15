#pragma once

#include "skin_c.h"

struct M2Geometry_s {
  glm::vec3 position;
  glm::vec3 normal;
  uint8_t bone_weight[4];
  uint8_t bone_indices[4];
  glm::vec2 texcoord;
  glm::vec2 unknown;
};

typedef std::vector<M2Geometry_s> M2Geometry_t;

/*! \brief M2 file handler. */
class M2_c : public Chunk_c {
 public:
  M2_c(Buffer_t *buffer);
  virtual ~M2_c();

  void GetMesh(const Skin_c &skin, Mesh_s *mesh) const;
  void GetBVMesh(Mesh_s *mesh) const;

  Skin_c *skin;

 private:
  void GetVertices(Points_t *vertices) const;
  void GetNormals(Points_t *normals) const;

  M2Geometry_t content_;
};

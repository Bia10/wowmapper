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

#pragma once

#include "../chunk_c.h"

struct M2Vertex_s {
    glm::vec3 position;
    glm::vec3 normal;
    uint8_t bone_weight[4];
    uint8_t bone_indices[4];
    glm::vec2 texcoord;
    glm::vec2 unknown;
  };

typedef std::vector<M2Vertex_s> M2Vertices_t;

/*! \brief M2 file handler. */
class M2_c : public Chunk_c {
 public:
  M2_c(const uint8_t *buffer, uint32_t length);

  void get_vertex_buffer(Points_t *buffer) const;
  void get_normal_buffer(Points_t *buffer) const;

  const M2Vertices_t& vertices() const { return vertices_; }
  const Points_t& bv_vertices() const { return bv_vertices_; }
  const Indices16_t& bv_indices() const { return bv_indices_; }

 private:
  std::string model_name_;
  uint32_t model_flags_;

  M2Vertices_t vertices_;
  Indices16_t bv_indices_;  //!< bounding volume indices
  Points_t bv_vertices_;    //!< bounding volume vertices
};

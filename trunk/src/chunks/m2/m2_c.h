#pragma once

#include "../chunk_c.h"

class M2_c : public Chunk_c {
 public:
  M2_c(const uint8_t *buffer, uint32_t length);

 private:
  struct M2Vertex_s {
    glm::vec3 position;
    glm::vec3 normal;
    uint8_t bone_weight[4];
    uint8_t bone_indices[4];
    glm::vec2 texcoord;
    glm::vec2 unknown;
  };
  typedef std::vector<M2Vertex_s> M2Vertices_t;

  std::string model_name_;
  uint32_t model_flags_;

  M2Vertices_t m2_vertices_;
};

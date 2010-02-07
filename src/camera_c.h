#pragma once

#include <glm/glm.hpp>

class Camera_c {
 public:
  Camera_c(glm::vec3 const &position = glm::vec3(0.0f, 0.0f, 0.0f));

  glm::vec3& position() { return position_; }
  glm::vec2& rotation() { return rotation_; }

 private:
  glm::vec3 position_;
  glm::vec2 rotation_;
};

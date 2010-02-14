#pragma once

#include "common.h"

class Camera_c {
 public:
  Camera_c(glm::vec3 const &position = glm::vec3(0.0f, 0.0f, 0.0f));

  glm::vec3& position() { return position_; }
  glm::vec2& rotation() { return rotation_; }
  glm::vec3& direction() { return direction_; }

 private:
  glm::vec3 position_;
  glm::vec2 rotation_;
  glm::vec3 direction_;
};

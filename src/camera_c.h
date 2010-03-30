#pragma once

#include "common.h"

class Camera_c {
 public:
  Camera_c(const glm::vec3 &position);

  void Rotate(float mx, float my);
  void SetMousePos(float mx, float my);
  void Walk(float speed);
  void Strafe(float speed);
  void SwimUp(float speed);

  const glm::vec3& position() const { return position_; }
  const glm::vec3& lookat() const { return position_+direction_; }
  const glm::vec3& direction() const { return direction_; }

 private:
  glm::vec3 position_;
  glm::vec3 direction_;
  glm::vec2 mouse_pos_;
  glm::vec2 yaw_pitch_;
};

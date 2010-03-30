#include "camera_c.h"

Camera_c::Camera_c(const glm::vec3 &position)
    : position_(position),
      direction_(0.0f, 0.0f, 0.0f),
      mouse_pos_(0.0f, 0.0f),
      yaw_pitch_(0.0f, 0.0f) {
}

void Camera_c::Rotate(float mx, float my) {
  yaw_pitch_ += (glm::vec2(mx, my) - mouse_pos_);

  glm::mat4 rot_mtx;
  rot_mtx = glm::rotate(rot_mtx, yaw_pitch_.y, glm::vec3(1, 0, 0));
  rot_mtx = glm::rotate(rot_mtx, yaw_pitch_.x, glm::vec3(0, 1, 0));

  direction_ = glm::vec3(0, 0, 1)*glm::mat3(rot_mtx);
}

void Camera_c::SetMousePos(float mx, float my) {
  mouse_pos_ = glm::vec2(mx, my);
}

void Camera_c::Walk(float speed) {
  position_ += direction_ * speed;
}

void Camera_c::Strafe(float speed) {
  glm::vec3 dir_n = glm::normalize(glm::vec3(-direction_.z, 0.0f, direction_.x));
  position_ +=  dir_n * speed;
}

void Camera_c::SwimUp(float speed) {
  glm::mat4 rot_mtx;
  rot_mtx = glm::rotate(rot_mtx, yaw_pitch_.y, glm::vec3(1, 0, 0));
  rot_mtx = glm::rotate(rot_mtx, yaw_pitch_.x, glm::vec3(0, 1, 0));

  position_ += direction_ + glm::vec3(0, speed, 0)*glm::mat3(rot_mtx);
}

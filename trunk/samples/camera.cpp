#include "camera.h"

//------------------------------------------------------------------------------
Camera::Camera( const glm::vec3 &pos, const glm::vec3 &look_at )
    : _position( pos ),
      _lookAt( look_at ) {
  _direction = _lookAt - _position;
}

//------------------------------------------------------------------------------
void Camera::rotate( float yaw, float pitch ) {
  // create rotation
  glm::quat q_rotate;
  q_rotate = glm::gtc::quaternion::rotate( q_rotate, yaw, glm::vec3( 0, 1, 0 ) );
  q_rotate = glm::gtc::quaternion::rotate( q_rotate, -pitch, glm::vec3( 1, 0, 0 ) );

  _direction = q_rotate * _direction;
  _lookAt = _position + _direction;
}

//------------------------------------------------------------------------------
void Camera::move( float value ) {
  // normalize vector and multiply by value
  glm::vec3 norm = glm::normalize( _direction );
  norm *= value;
  // update position and lookAt
  _position += norm;
  _lookAt += norm;
}

//------------------------------------------------------------------------------
const glm::vec3& Camera::position() const {
  return _position;
}

//------------------------------------------------------------------------------
const glm::vec3& Camera::lookAt() const {
  return _lookAt;
}

//------------------------------------------------------------------------------
void Camera::setPosition( const glm::vec3 &pos ) {
  _position = pos;
  _direction = _lookAt - _position;
}

//------------------------------------------------------------------------------
void Camera::setLookAt( const glm::vec3 &look_at ) {
  _lookAt = look_at;
  _direction = _lookAt - _position;
}
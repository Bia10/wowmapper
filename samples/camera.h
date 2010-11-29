/*  wowmapper - World of Warcraft MAP ParsER
    Copyright (C) 2010 PferdOne aka Flowerew

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

#include "../src/common.h"

class Camera {
 public:
  Camera( const glm::vec3 &pos, const glm::vec3 &look_at );
  void rotate( float yaw, float pitch );
  void move( float value );
  const glm::vec3& position() const;
  const glm::vec3& lookAt() const;
  void setPosition( const glm::vec3 &pos );
  void setLookAt( const glm::vec3 &look_at );

 private:
  glm::vec3 _position;
  glm::vec3 _lookAt;
  glm::vec3 _direction;
};
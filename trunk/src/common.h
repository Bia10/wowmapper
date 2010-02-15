#pragma once

#define SAFE_DELETE(p) { delete p; p = NULL; }

#include <map>
#include <vector>
#include <list>
#include <algorithm>
#include <string>
#include <stdint.h>
#include <iostream>
#include <exception>
#include <memory>

#include <glm/glm.hpp>

typedef std::vector<glm::vec3> Points_t;
typedef std::vector<uint16_t> Indices16_t;
typedef std::vector<uint32_t> Indices32_t;

/* FUNCTORS */
/*! @brief Functor to swap axes based on wowdev info: X, Z, -Y
 *         Applies to wmo vertices & normals. */
struct SwapAxes_ftor {
  glm::vec3 operator()(glm::vec3 &point) {
      float temp = point.y;
      point.y = point.z;
      point.z = -temp;
      return point;
  }
};

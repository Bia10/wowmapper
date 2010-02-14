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

#include <glm/glm.hpp>

typedef std::vector<glm::vec3> Points_t;
typedef std::vector<uint16_t> Indices16_t;
typedef std::vector<uint32_t> Indices32_t;

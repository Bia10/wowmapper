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
#include <time.h>
#include <iterator>
#include <bitset>
#include <sstream>

#include <glm/glm.hpp>

/* TYPEDEFS */
typedef std::vector<glm::vec3>    Points_t;
typedef std::vector<uint16_t>     Indices16_t;
typedef std::vector<uint32_t>     Indices32_t;

typedef std::vector<uint8_t>      Buffer_t;
typedef std::vector<std::string>  Filenames_t;

typedef std::bitset<8>            BitSet8_t;
typedef std::bitset<16>           BitSet16_t;
typedef std::bitset<32>           BitSet32_t;


/* FUNCTIONS */
/*! \brief Function to swap axes based on wowdev info: X, Z, -Y
 *         Applies to wmo/adt vertices & normals. */
static glm::vec3 SwapAxes(glm::vec3 &vec) {
  return glm::vec3(vec.x, vec.z, -vec.y);
}

#define UNIT 2.083333333f // 100.0f / (3.0f * 16.0f);

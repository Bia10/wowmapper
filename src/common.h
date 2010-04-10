#pragma once

#define SAFE_DELETE(p) { delete p; p = NULL; }

#include <map>
#include <vector>
#include <list>
#include <algorithm>
#include <string>
#include <iostream>
#include <exception>
#include <memory>
#include <iterator>
#include <bitset>
#include <sstream>

#include <dirent.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

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

template<typename T>
static void InsertIndices(const std::vector<T> &ins, T offset, std::vector<T> *out) {
  std::vector<T> ins_cpy(ins.begin(), ins.end());

  // increment index values by offset
  std::vector<T> add(ins.size(), offset);
  std::transform(ins_cpy.begin(), ins_cpy.end(), add.begin(),
      ins_cpy.begin(), std::plus<T>());

  out->insert(out->end(), ins_cpy.begin(), ins_cpy.end());
}

static std::string ToLower(const std::string &str) {
  std::string lower(str);
  std::transform(lower.begin(), lower.end(), lower.begin(), tolower);
  return lower;
}

#define UNIT 2.083333333f // 100.0f / (3.0f * 16.0f);

struct BlockInfo_s {
  int32_t num;
  int32_t offset;
};

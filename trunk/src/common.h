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
#include <fstream>
#include <float.h>

#include <dirent.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <pthread.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

/* TYPEDEFS */
typedef std::vector<glm::vec3>    Points_t;
typedef std::vector<glm::vec2>    TexCoords_t;
typedef std::vector<uint16_t>     Indices16_t;
typedef std::vector<uint32_t>     Indices32_t;

typedef std::vector<uint8_t>      Buffer_t;
typedef std::vector<std::string>  Filenames_t;

typedef std::bitset<8>            BitSet8_t;
typedef std::bitset<16>           BitSet16_t;
typedef std::bitset<32>           BitSet32_t;

#define TU 2.083333333f // Tile Units, 100.0f / (3.0f * 16.0f);

struct BlockInfo_s {
  int32_t num;
  int32_t offset;
};

/*! \brief Bounding box. */
struct BBox_s {
  glm::vec3 min, max;
};

/*! \brief Mesh containing geometry. */
struct Mesh_s {
  Points_t vtx;
  Points_t norm;
  Indices32_t idx;
  Indices32_t col;
  TexCoords_t txc;
  BBox_s bbox;
};

typedef std::vector<Mesh_s> Meshes_t;


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

/*! \brief Transforms WoW coordinates to RH coordinates.
 *  \param pos Translation.
 *  \param rot Rotation.
 *  \param scale Scale.
 *  \param vertices Vertices to be transformed. */
static void TransWoWToRH(const glm::vec3 &pos,
                         const glm::vec3 &rot,
                         float scale,
                         Points_t *vertices) {
  // rotation
  glm::mat4 rot_mtx;
  rot_mtx = glm::rotate(rot_mtx, rot.y-90, glm::vec3(0, 1, 0));
  rot_mtx = glm::rotate(rot_mtx, -rot.x, glm::vec3(0, 0, 1));
  rot_mtx = glm::rotate(rot_mtx, rot.z-90, glm::vec3(1, 0, 0));

  // translation
  glm::vec3 transl(pos.x-17066.666666f, pos.y, pos.z-17066.666666f);

  // transform vertices
  for (Points_t::iterator vtx = vertices->begin();
       vtx != vertices->end();
       ++vtx) {
    glm::vec4 vtx4(*vtx, 0);
    vtx4 = scale * rot_mtx * vtx4;
    *vtx = glm::vec3(vtx4) + pos;
  }
}

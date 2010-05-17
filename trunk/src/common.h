#pragma once

#include <map>
#include <vector>
#include <list>
#include <algorithm>
#include <string>
#include <iostream>
#include <exception>
#include <memory>
#include <iterator>
#include <sstream>
#include <fstream>
#include <float.h>
#include <limits>

#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "types.h"


#define TU 100.0f / (3.0f * 16.0f) // Tile Units

struct BlockInfo_s {
  wm_size_t num;
  wm_off_t offset;
};

// model map
class Model_c;
typedef std::map<std::string, Model_c*> ModelMap_t;
typedef std::pair<std::string, Model_c*> ModelPair_t;

/*! \brief Template function used to insert a new set of indices into already
 *         existing indices, with an offset to make them align properly.
 *  \param in Indices to add to out.
 *  \param offset Offset to make them allign to out's indices.
 *  \param out Indices which will get in added. */
template<typename T>
static void InsertIndices(const std::vector<T> &ins, wm_off_t offset, std::vector<T> *out) {
  std::vector<T> ins_cpy(ins.begin(), ins.end());

  // increment index values by offset
  std::vector<T> add(ins.size(), offset);
  std::transform(ins_cpy.begin(), ins_cpy.end(), add.begin(),
      ins_cpy.begin(), std::plus<T>());

  out->insert(out->end(), ins_cpy.begin(), ins_cpy.end());
}

/*! \brief Used to replace WoWs extensions on files (cause some or wrong!!!). */
static bool RreplaceWoWExt(const std::string &in, const std::string &old,
                        const std::string &rep, std::string *out) {
  wm_size_t last_bslash = in.rfind('\\');  // used to fix a single file: meatwagonhauler!
  wm_size_t pos = in.rfind(old);
  if (last_bslash < pos && pos != std::string::npos) {
    *out = in;
    out->replace(pos, old.length(), rep);
    return true;
  } else {
    return false;
  }
}

/*! \brief Returns a string of lower characters. */
static std::string ToLower(const std::string &str) {
  std::string lower(str);
  std::transform(lower.begin(), lower.end(), lower.begin(), tolower);
  return lower;
}

static void TranslateVertices(const glm::vec3 &pos, Vertices_t *vertices,
                              wm_off_t off, wm_size_t num) {
  // transform vertices
  for (wm_size_t i = off; i < (off+num); i++) {
    (*vertices)[i] += pos;
  }
}

/*! \brief Transforms WoW coordinates to RH coordinates.
 *  \param pos Translation.
 *  \param rot Rotation.
 *  \param scale Scale.
 *  \param vertices Vertices to be transformed. */
static void TransformVertices(const glm::vec3 &pos, const glm::vec3 &rot,
                              float scale, Vertices_t *vertices,
                              wm_off_t off, wm_size_t num) {
  // rotation
  glm::mat4 rot_mtx;
  rot_mtx = glm::rotate(rot_mtx, rot.y, glm::vec3(0, 1, 0));
  rot_mtx = glm::rotate(rot_mtx, rot.x, glm::vec3(0, 0, 1));
  rot_mtx = glm::rotate(rot_mtx, rot.z, glm::vec3(1, 0, 0));

  // transform vertices
  for (wm_size_t i = off; i < (off+num); i++) {
    glm::vec3 &vtx = (*vertices)[i];
    glm::vec4 vtx4(vtx, 0);
    vtx4 = scale * rot_mtx * vtx4;
    vtx = glm::vec3(vtx4) + pos;
  }
}

static void TransformVertices(const glm::vec3 &pos, const glm::quat &rot,
                              float scale, Vertices_t *vertices,
                              wm_off_t off, wm_size_t num) {
  // transform vertices
  for (wm_size_t i = off; i < (off+num); i++) {
    glm::vec3 &vtx = (*vertices)[i];
    vtx = glm::gtx::quaternion::rotate(rot, vtx) * scale + pos;
  }
}

/*! \brief This function will rearrange your geometry buffer based on marked indices.
 *         Marked indices are indices which are set to: uint_max == -1 == 0xffffffff */
static void RearrangeBuffers(Indices32_t *indices, Vertices_t *vertices,
                             Normals_t *normals) {
  Indices32_t idx_map(indices->size(), 0xffffffff); // indices remapped
  Indices32_t new_idx;                              // new indices
  new_idx.reserve(indices->size());

  // new vertices and normals
  Vertices_t new_vtx;
  Normals_t new_norm;
  new_vtx.reserve(vertices->size());
  new_norm.reserve(normals->size());

  wm_size_t new_count = 0;
  for(wm_size_t i = 0; i < indices->size(); i++) {
    uint32_t marked_index = (*indices)[i];
    // check if index is marked
    if (marked_index != 0xffffffff) {
      // if not check for an already new index in the index map
      if (idx_map[marked_index] == 0xffffffff) {
        // we have a new index so map the new value and insert vtx and norms
        idx_map[marked_index] = new_count;
        new_vtx.push_back((*vertices)[marked_index]);
        new_norm.push_back((*normals)[marked_index]);
        new_count++;
      }
      // push mapped index value to new index array
      new_idx.push_back(idx_map[marked_index]);
    }
  }

  indices->swap(new_idx);
  vertices->swap(new_vtx);
  normals->swap(new_norm);
}

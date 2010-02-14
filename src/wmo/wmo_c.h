#pragma once

#include "mohdchunk_s.h"
#include "mognchunk_s.h"
#include "subwmo_c.h"
#include "../mpqhandler_c.h"

typedef std::list<std::string> SubWmoNames_t;
typedef std::list<SubWmo_c*> SubWmoList_t;

/*! @brief Class to deal with WMO files. */
class Wmo_c {
 public:
  Wmo_c(uint8_t **buffer, const char *name, MpqHandler_c *mpqHandler = NULL);

  const SubWmoList_t& sub_wmo_list() const { return sub_wmo_list_; }
  const MohdChunk_s& mohd_chunk() const { return mohd_chunk_; }

  const Points_t& vertices() const { return vertices_; }
  const Points_t& normals() const { return normals_; }
  const Indices32_t& indices() const { return indices_; }
  const std::string& name() const { return name_; }

 private:
  void PrintDetails() const;
  /*! @brief Create filenames for sub WMOs. */
  void GenerateSubWmoNames();
  /*! @brief Load sub wmo files and return vertex, normals and index count. */
  void LoadSubWmos(MpqHandler_c *mpqHandler);
  /*! @brief Combine all sub wmos into one single index, vertex, normals, list. */
  void CombineSubWmos();

  std::string name_;
  MohdChunk_s mohd_chunk_;

  SubWmoNames_t sub_wmo_names_;
  SubWmoList_t sub_wmo_list_;

  Points_t vertices_;
  Points_t normals_;
  Indices32_t indices_;
};

#pragma once

#include "../chunk_c.h"

/*! \brief MODD: Map Object Doodad Information. */
struct ModdChunk_s : public Chunk_c {
  struct DoodadInfo_s {
    off_t name_off;           //!< Offset to name
    glm::vec3 pos;            //!< Position
    glm::quat rot;            //!< Rotation (quaternion)
    float scale;              //!< Scale
    uint32_t lighting_color;  //!< Lighting Color
  };
  typedef std::vector<DoodadInfo_s> DoodadInfos_t;

  DoodadInfos_t doodad_infos;


  ModdChunk_s(Chunk_c *parent, off_t off)
      : Chunk_c(parent, off) {
    size_t num_infos = GetSize() / sizeof(DoodadInfo_s);
    doodad_infos.resize(num_infos);

    CopyVector(GetBuffer(), GetCurOffset()+DATA_OFFSET, num_infos, &doodad_infos);
  }
};

#include "wmo_c.h"

Wmo_c::Wmo_c(Buffer_t *buffer, std::string wmo_name, MpqHandler_c &mpq_h)
    : Model_c(buffer), mohd_(this), mogn_(this),
      mogi_(this), mods_(this), modd_(this) {
  GetSubChunk("DHOM", &mohd_);
  GetSubChunk("NGOM", &mogn_);
  GetSubChunk("IGOM", &mogi_);
  GetSubChunk("SDOM", &mods_);
  GetSubChunk("DDOM", &modd_);

  // get subwmo triangles
  for (uint32_t i = 0; i < mohd_.num_groups; i++) {
    char num[5]; sprintf(num, "_%03d", i);
    std::string subwmo_name(wmo_name.substr(0, wmo_name.size()-4) + num + std::string(".wmo"));

    Buffer_t subwmo_buf;
    mpq_h.LoadFile(subwmo_name.c_str(), &subwmo_buf);
    SubWmo_c subwmo(&subwmo_buf);

    Indices32_t indices(subwmo.indices().begin(), subwmo.indices().end());
    MopyChunk_s::MaterialInfo_t::const_iterator info = subwmo.mopy().mat_info.begin();
    for (uint32_t i = 0; i < indices.size()/3; i++, info++) {
      if (info->flags & 0x4) {
        indices[i*3+0] = 0;
        indices[i*3+1] = 0;
        indices[i*3+2] = 0;
      }
    }

    InsertIndices(indices, vertices_.size(), &indices_);
    vertices_.insert(vertices_.end(), subwmo.vertices().begin(), subwmo.vertices().end());
    normals_.insert(normals_.end(), subwmo.normals().begin(), subwmo.normals().end());
  }

  colors_.assign(vertices_.size(), 0xff0066cc);
}

#include "wmo_c.h"
#include "moddchunk_s.h"
#include "modnchunk_s.h"

Wmo_c::Wmo_c(uint8_t **buffer, const char *name, MpqHandler_c *mpqHandler /* = NULL */)
    : name_(name) {
 /* uint32_t offset = 0xC + mohd_chunk_.total_size();
  for(int i = 0; i < 12; i++) {
    offset += Chunk_s(offset, *buffer, false).total_size();
  }

  ModnChunk_s modn(offset, *buffer);
  offset += modn.total_size();
  ModdChunk_s modd(offset, *buffer);*/

  delete [] *buffer;
  *buffer = NULL;

  /* print some wmo details and return if we have no mpq handler */
  PrintDetails();

  if (mpqHandler == NULL) return;

  GenerateSubWmoNames();
  LoadSubWmos(mpqHandler);
  CombineSubWmos();
}

void Wmo_c::PrintDetails() const {
  std::cout << "WMO details: ";
  std::cout << "ID: " << mohd_chunk_.wmo_id << ", ";
  std::cout << "Groups: " << mohd_chunk_.num_groups << ", ";
  std::cout << "Portals: " << mohd_chunk_.num_portals << std::endl;
}

void Wmo_c::GenerateSubWmoNames() {
  char group_num[9]; // will contain sub wmo extension "_###.wmo"

  for (uint32_t i = 0; i < mohd_chunk_.num_groups; i++) {
    //snprintf(group_num, 9, "_%03d.wmo", i);
    std::string sub_wmo_name = name_.substr(0, name_.size() - 4);
    /* create final string: my\\path\\WmoName_00i.wmo */
    sub_wmo_name.append(group_num);
    sub_wmo_names_.push_back(sub_wmo_name);
  }
}

void Wmo_c::LoadSubWmos(MpqHandler_c *mpqHandler) {
  for (SubWmoNames_t::iterator sub_wmo_name = sub_wmo_names_.begin(); sub_wmo_name
      != sub_wmo_names_.end(); ++sub_wmo_name) {
    uint8_t *sub_wmo_buffer = NULL;
    mpqHandler->LoadFileByName(sub_wmo_name->c_str(), &sub_wmo_buffer);
    sub_wmo_list_.push_back(new SubWmo_c(&sub_wmo_buffer, sub_wmo_name->c_str()));
  }
}

void Wmo_c::CombineSubWmos() {
  /* fill vertex, normal and index array with values */
  for (SubWmoList_t::iterator sub_wmo = sub_wmo_list_.begin();
       sub_wmo != sub_wmo_list_.end();
       ++sub_wmo) {
    /* insert sub wmo vertices into vertices */
    //const Points_t &vertices = (*sub_wmo)->movt_chunk()->vertices;
    //vertices_.insert(vertices_.end(), vertices.begin(), vertices.end());

    //const Points_t &normals = (*sub_wmo)->monr_chunk()->normals;
    //normals_.insert(normals_.end(), normals.begin(), normals.end());

    /* copy 16bit indices to 32bit indices */
    //const Indices16_t &movi_indices = (*sub_wmo)->movi_chunk()->indices;
    //Indices32_t indices(movi_indices.begin(), movi_indices.end());

    /* increase index by an offset if we have more than one on sub wmo */
    /*uint32_t offset = vertices_.size() - vertices.size();
    if (offset > 0) {
      std::vector<uint32_t> diff(indices.size(), offset);
      std::transform(indices.begin(), indices.end(),
                     diff.begin(), indices.begin(), std::plus<uint32_t>());
    }
    indices_.insert(indices_.end(), indices.begin(), indices.end());*/
  }
}

#include "adt_c.h"

#define MHDR_OFFSET 0xC
#define STEP 2.083333333f       // 100.0f / (3.0f * 16.0f);
#define XZOFFSET 533.333333333f

Adt_c::Adt_c(uint8_t **buffer, MpqHandler_c *mpqHandler /* = NULL */) :
  mhdr_chunk_(MHDR_OFFSET, *buffer) {
  delete [] *buffer;
  *buffer = NULL; // invalidate buffer

  if(mpqHandler == NULL) return;

  LoadWmo(mpqHandler);
}

void Adt_c::GenerateMesh() {
  for (int idx = 0; idx < 256; idx++) {
    McnkChunk_s &mcnk = *mhdr_chunk_.mcin->mcnk_index[idx].mcnk;
    glm::vec3 mcnk_pos(mcnk.position.y, mcnk.position.z, mcnk.position.x);

    /* a mcnk chunk contains 8x8 patches of 5 vertices each patch
     * 1 --- 2
     *   |X|
     * 3 --- 4 */
    for (int y = 0; y < 8; y++) {
      for (int x = 0; x < 8; x++) {
        const glm::vec3 *vertices = (*mcnk.mcvt)[y * 8 + x];
        const glm::vec3 *normals = (*mcnk.mcnr)[y * 8 + x];

        /* since all vertices are stored next to each other we have to
         * calculate our current index to retrieve the right ones:
         * MCNK_index * 256 * 3vertices + (row * 8 + col) * num_vertices_per_patch */
        int32_t cur_idx = idx * 256 * 3 + (y * 8 + x) * 12;

        for (int off = 0; off < 12; off++) {
          /* the map files have their X & Z axes reversed, so we have to: */
          glm::vec3 vert = glm::vec3(32*XZOFFSET - (mcnk_pos.x - vertices[off].x),
                                     vertices[off].y + mcnk_pos.y,
                                     32*XZOFFSET - (mcnk_pos.z - vertices[off].z));

          vertices_[cur_idx + off] = vert;
          normals_[cur_idx + off] = normals[off];
        }
      }
    }
  }
}

void Adt_c::LoadWmo(MpqHandler_c *mpqHandler) {
  uint8_t *wmo_buffer = NULL;

  /* begin loading of wmos here */
  for (WmoFilenames_t::iterator wmo_name = mhdr_chunk_.mwmo->filenames.begin();
       wmo_name != mhdr_chunk_.mwmo->filenames.end();
       ++wmo_name) {
    std::cout << "Load WMO: " << *wmo_name << std::endl;

    mpqHandler->LoadFileByName(wmo_name->c_str(), &wmo_buffer);
    wmo_list_.push_back(new Wmo_c(&wmo_buffer, wmo_name->c_str(), mpqHandler));

    wmo_buffer = NULL;
  }
}

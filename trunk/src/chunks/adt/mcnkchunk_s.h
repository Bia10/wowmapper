#pragma once

#include "mcvtchunk_s.h"
#include "mcnrchunk_s.h"
#include "mcrfchunk_s.h"

/*! \brief MCNK chunk.
 *  \url http://www.madx.dk/wowdev/wiki/index.php?title=ADT/v18#MCNK_chunk */
struct McnkChunk_s : public Chunk_c {
  uint32_t flags;
  uint32_t index_x;
  uint32_t index_y;
  uint32_t num_doodad_refs;
  uint32_t num_wmo_refs;
  uint32_t area_id;
  uint32_t holes;
  glm::vec3 position;
  McvtChunk_s mcvt;
  McnrChunk_s mcnr;
  McrfChunk_s mcrf;

  McnkChunk_s(Chunk_c *parent)
      : Chunk_c(parent), mcvt(this), mcnr(this), mcrf(this) { }

 protected:
  virtual void Initialize() {
    flags = GetField<uint32_t>(0x00);           // flags
    index_x = GetField<uint32_t>(0x04);         // index X
    index_y = GetField<uint32_t>(0x08);         // index X
    num_doodad_refs = GetField<uint32_t>(0x10); // number of doodad references
    num_wmo_refs = GetField<uint32_t>(0x38);    // number of wmo references
    area_id = GetField<uint32_t>(0x34);         // area ID
    holes = GetField<uint32_t>(0x3c);           // map holes bitmap
    position = GetField<glm::vec3>(0x68);       // position

    // initialize vertices, normals and doodad/wmo-references
    mcvt.SetPosition(position);
    mcvt.SetHoles(holes);
    GetSubChunk("TVCM", 4, &mcvt);

    GetSubChunk("RNCM", 4, &mcnr);

    // workaround :p
    if ((num_doodad_refs + num_wmo_refs) == 0) { return; }

    mcrf.SetNumRefs(num_doodad_refs, num_wmo_refs);
    GetSubChunk("FRCM", 4, &mcrf);
  }
};

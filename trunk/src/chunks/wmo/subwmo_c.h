#pragma once

#include "mogpchunk_s.h"
#include "mopychunk_s.h"
#include "movichunk_s.h"
#include "movtchunk_s.h"
#include "monrchunk_s.h"

/*! \brief Beside root WMOs there are group WMOs or sub WMOs, which contain the
 *         actual geometry data for the entire WMO. WMOs can consist of several
 *         group/sub WMOs. Best example is Stormwind. */
class SubWmo_c : public Chunk_c {
 public:
  SubWmo_c(Buffer_t *buffer);

  const Indices16_t& indices() const { return movi_.indices; }
  const Points_t& vertices() const { return movt_.vertices; }
  const Points_t& normals() const { return monr_.normals; }
  const MopyChunk_s& mopy() const { return mopy_; }

 private:
  MogpChunk_s mogp_;
  MopyChunk_s mopy_;
  MoviChunk_s movi_;
  MovtChunk_s movt_;
  MonrChunk_s monr_;
};

#pragma once


#include "common.h"
#include <libmpq/mpq.h>
#include <libmpq/mpq-internal.h>

typedef std::map<std::string, mpq_archive_s*> WowFileMap_t;
typedef std::pair<std::string, mpq_archive_s*> WowFile_t;
typedef std::list<mpq_archive_s*> MpqArchives_t;

/*! \brief MpqHandler simplifies mpq archive access. */
class MpqHandler_c {
 public:
  /*! \brief Initializes handler and sets search directory.
   *  \param searchDir Search directory */
  MpqHandler_c(const char *searchDir = ".");
  ~MpqHandler_c();
  /*! \brief Reads a file from our opened MPQs and returns it via buffer.
   *  \param filename Filename including full path of the file
   *  \param buffer File buffer
   *  \return True if file was successfully read, otherwise false. */
  bool LoadFile(const char *filename, Buffer_t *buffer) const;
  //int64_t LoadFileByName(const char *a, uint8_t **buffer) {}

 private:
  void GetMpqs();
  int32_t OpenMpq(const char *filename);

  std::string search_dir_;
  WowFileMap_t wow_file_map_;
  MpqArchives_t mpq_arcs_;
};

#pragma once


#include "common.h"
#include <libmpq/mpq.h>

typedef std::list<std::string> MpqFileList_t;

/*! \brief MpqHandler simplifies mpq archive access. */
class MpqHandler_c {
 public:
  /*! \brief Initializes handler and sets default directory.
   *
   *  @param defaultDir Default directory string
   */
  MpqHandler_c(const char *defaultDir = "./");
  ~MpqHandler_c();
  /*! @brief Set default directory to search for MPQs.
   *
   *  @param directory Default directory string
   */
  void SetDefaultDirectory(const char *directory);
  /*! @brief Opens a MPQ file from "default directory + filename".
   *
   *  @param filename MPQ filename
   *  @return Returns error code. Error code != 0 >> fail.
   */
  int32_t OpenFile(const char *filename);
  /*! @brief Returns a map of file indices and names which match the filter.
   *
   * @param filter Filter key: ".adt" will return all files containing ".adt"
   * @param outMap Pointer to MpqIndexNameMap
   * @return Returns amount of entries which matched the filter.
   */
  int32_t GetFileListByFilter(const char *filter, MpqFileList_t *outList) const;
  /*! @brief Reads a file from our opened MPQ and returns it via buffer.
   *
   * @param filename Full path name of the file
   * @param buffer If (*buffer) == NULL the function allocates memory for you
   * @return Returns number of bytes written to the buffer.
   */
  int64_t LoadFileByName(const char *filename, uint8_t **buffer) const;

  const MpqFileList_t& mpq_file_list() const { return mpq_file_list_; }
  mpq_archive_s* mpq_arc() const { return mpq_arc_; }

 private:

  std::string default_dir_;
  mpq_archive_s *mpq_arc_;
  MpqFileList_t mpq_file_list_;
};

#include "mpqhandler_c.h"
#include <iostream>
#include <libmpq/mpq-internal.h>
#include <string.h>

MpqHandler_c::MpqHandler_c(const char *defaultDir)
    : default_dir_("./"),
      mpq_arc_(NULL) {
  Initialize();
}

MpqHandler_c::~MpqHandler_c() {
  libmpq__archive_close(mpq_arc_);
  libmpq__shutdown();
}

bool MpqHandler_c::Initialize() {
  printf("Init libmpq version %s\n", libmpq__version());
  return libmpq__init() == LIBMPQ_SUCCESS;
}

void MpqHandler_c::SetDefaultDirectory(const char *directory) {
  default_dir_ = std::string(directory);
}

int32_t MpqHandler_c::OpenFile(const char *filename) {
  /* construct path from default directory and filename */
  std::string mpq_path(default_dir_);
  mpq_path.append(filename);

  int32_t err = 0;
  uint8_t *buf = NULL;

  try {
    /* open mpq archive */
    err = libmpq__archive_open(&mpq_arc_, mpq_path.c_str(), -1);
    if (err != LIBMPQ_SUCCESS)
      throw(std::string("libmpq__archive_open failed"));

    /* get file number of the list file which contains all names in the archive */
    uint32_t file_num = 0;
    err = libmpq__file_number(mpq_arc_, LIBMPQ_LISTFILE_NAME, &file_num);
    if (err != LIBMPQ_SUCCESS)
      throw(std::string("libmpq__file_number failed"));

    /* read file from archive...get file size and reserve space */
    int64_t filesize;
    err = libmpq__file_unpacked_size(mpq_arc_, file_num, &filesize);
    if(err != LIBMPQ_SUCCESS)
      throw(std::string("libmpq__file_unpacked_size failed"));

    /* read file from archive to buffer*/
    buf = new uint8_t[filesize];
    err = libmpq__file_read(mpq_arc_, file_num, buf, filesize, NULL);
    if (err != LIBMPQ_SUCCESS)
      throw(std::string("libmpq__file_read failed"));

  } catch(std::string err_msg) {
    std::cout << err_msg << " (" << err << ")" << std::endl;

    delete [] buf;
    libmpq__archive_close(mpq_arc_);

    return err;
  }

  /* get all filenames from listfile */
  for(char *arc_filename = strtok((char*)buf, "\r\n");
      arc_filename != NULL;
      arc_filename = strtok(NULL, "\r\n")) {
    mpq_file_list_.push_back(arc_filename);
  }

  delete [] buf;

  return err;
}

int32_t MpqHandler_c::GetFileListByFilter(const char *filter,
                                          MpqFileList *outList) const {
  int32_t num_files = 0;
  /* insert into outList if we find our filter in the string */
  for(MpqFileList::const_iterator mpq_file = mpq_file_list_.begin();
      mpq_file != mpq_file_list_.end();
      ++mpq_file) {
    if (mpq_file->find(filter, 0) != std::string::npos) {
      outList->push_back(*mpq_file);
      num_files++;
    }
  }

  return num_files;
}

int64_t MpqHandler_c::LoadFileByName(const char *filename,
                                     uint8_t **buffer) const {
  int64_t unpacked_size = 0;
  int64_t written_bytes = 0;
  uint32_t file_num = 0;

  libmpq__file_number(mpq_arc_, filename, &file_num);
  libmpq__file_unpacked_size(mpq_arc_, file_num, &unpacked_size);

  /* create buffer if it's null-pointer (NULL) */
  if (*buffer == NULL)
    *buffer = new uint8_t[unpacked_size];

  libmpq__file_read(mpq_arc_, file_num, *buffer, unpacked_size, &written_bytes);

  return written_bytes;
}

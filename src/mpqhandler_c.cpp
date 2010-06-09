#include "mpqhandler_c.h"
#ifdef WIN32
 #include <Windows.h>
#endif

MpqHandler_c::MpqHandler_c(const std::string &searchDir)
    : search_dir_(searchDir) {
  GetMpqs();
}

MpqHandler_c::~MpqHandler_c() {
  for (MpqArchives_t::iterator arc = mpq_arcs_.begin();
       arc != mpq_arcs_.end();
       ++arc) {
    libmpq__archive_close(*arc);
  }
}

int32_t MpqHandler_c::OpenMpq(const std::string &filename) {
  // construct path from default directory and filename
#ifdef WIN32
  std::string mpq_path(filename);
#else
  std::string mpq_path(search_dir_ + "/" + filename);
#endif

  int32_t err = 0;
  uint8_t *buf = NULL;
  mpq_archive_s *mpq_arc = NULL;

  try {
    // open mpq archive
    err = libmpq__archive_open(&mpq_arc, mpq_path.c_str(), -1);
    if (err != LIBMPQ_SUCCESS) {
      throw(std::string("libmpq__archive_open failed"));
    }

    // get file number of the list file which contains all names in the archive
    uint32_t file_num = 0;
    err = libmpq__file_number(mpq_arc, LIBMPQ_LISTFILE_NAME, &file_num);
    if (err != LIBMPQ_SUCCESS) {
      throw(std::string("libmpq__file_number failed"));
    }

    //read file from archive...get file size and reserve space
    libmpq__off_t filesize;
    err = libmpq__file_unpacked_size(mpq_arc, file_num, &filesize);
    if(err != LIBMPQ_SUCCESS) {
      throw(std::string("libmpq__file_unpacked_size failed"));
    }

    // read file from archive to buffer
    buf = new uint8_t[filesize];
    err = libmpq__file_read(mpq_arc, file_num, buf, filesize, NULL);
    if (err != LIBMPQ_SUCCESS) {
      throw(std::string("libmpq__file_read failed"));
    }

  } catch(std::string err_msg) {
    std::cout << err_msg << " (" << err << ")" << std::endl;

    delete [] buf;

    if (mpq_arc) {
      libmpq__archive_close(mpq_arc);
    }

    return err;
  }

  // get all filenames (lowercase) from listfile
  for(char *wow_filename = strtok((char*)buf, "\r\n");
      wow_filename != NULL;
      wow_filename = strtok(NULL, "\r\n")) {
    wow_file_map_.insert(WowFile_t(ToLower(wow_filename), mpq_arc));
  }

  mpq_arcs_.push_back(mpq_arc);

  delete [] buf;

  return err;
}

#ifdef WIN32
void MpqHandler_c::GetMpqs() {
  // retrieve full path name
  const int MAX_PATH_LENGTH = 256;
  char path_name[MAX_PATH_LENGTH];
  GetFullPathName(search_dir_.c_str(), MAX_PATH_LENGTH, path_name, NULL);

  // append search mask
  std::string mpq_dir(path_name);
  mpq_dir.append("\\*.mpq");  

  WIN32_FIND_DATA file_data;
  HANDLE find_h = FindFirstFile(mpq_dir.c_str(), &file_data);

  if(find_h == INVALID_HANDLE_VALUE) {
    std::cout << "Cannot find file/directory." << std::endl;
    return;
  }

  Strings_t mpq_names;
  mpq_names.push_back(search_dir_ + "\\" + std::string(file_data.cFileName));

  while(FindNextFile(find_h, &file_data)) {
    mpq_names.push_back(search_dir_ + "\\" + std::string(file_data.cFileName));
  }

  FindClose(find_h);
  
  std::sort(mpq_names.begin(), mpq_names.end());

  for (Strings_t::reverse_iterator mpq_name = mpq_names.rbegin();
       mpq_name != mpq_names.rend();
       ++mpq_name) {
    std::cout << "Open MPQ: " << *mpq_name << std::endl;
    OpenMpq(mpq_name->c_str());    
  }

  std::cout << "Files in MPQs available: " << wow_file_map_.size() << std::endl;
}
#else
void MpqHandler_c::GetMpqs() {
  DIR *dir = opendir(search_dir_.c_str());

  Strings_t mpq_names;
  // list everything what's in the search dir and open mpq if we find one
  for (dirent *dir_entry = readdir(dir); dir_entry; dir_entry = readdir(dir)) {
    std::string cur_name(dir_entry->d_name);
    std::transform(cur_name.begin(), cur_name.end(), cur_name.begin(), tolower);

    // found a mpq file and load its file list
    if (cur_name.find(".mpq", 0) != std::string::npos) {
      mpq_names.push_back(dir_entry->d_name);
    }
  }

  std::sort(mpq_names.begin(), mpq_names.end());

  for (Strings_t::reverse_iterator mpq_name = mpq_names.rbegin();
       mpq_name != mpq_names.rend();
       ++mpq_name) {
    OpenMpq(mpq_name->c_str());
    std::cout << "Open MPQ: " << *mpq_name << std::endl;
  }

  std::cout << "Files in MPQs available: " << wow_file_map_.size() << std::endl;

  closedir(dir);
}
#endif

bool MpqHandler_c::LoadFile(const std::string &filename, Buffer_t *buffer) const {
  libmpq__off_t unpacked = 0;
  libmpq__off_t bytes = 0;
  uint32_t file_num = 0;

  WowFileMap_t::const_iterator wow_file = wow_file_map_.find(ToLower(filename));

  if (wow_file != wow_file_map_.end()) {
    mpq_archive_s *mpq_arc = wow_file->second;
    libmpq__file_number(mpq_arc, filename.c_str(), &file_num);
    libmpq__file_unpacked_size(mpq_arc, file_num, &unpacked);

    buffer->resize(unpacked);
    libmpq__file_read(mpq_arc, file_num, &((*buffer)[0]), unpacked, &bytes);
  }

  return bytes > 0;
}

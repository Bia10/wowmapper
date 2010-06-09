/*  wowmapper - World of Warcraft MAP PARser
    Copyright (C) 2010 PferdOne

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>. */

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
  MpqHandler_c(const std::string &searchDir = ".");
  ~MpqHandler_c();
  /*! \brief Reads a file from our opened MPQs and returns it via buffer.
   *  \param filename Filename including full path of the file
   *  \param buffer File buffer
   *  \return True if file was successfully read, otherwise false. */
  bool LoadFile(const std::string &filename, Buffer_t *buffer) const;
  //int64_t LoadFileByName(const char *a, uint8_t **buffer) {}

 private:
  void GetMpqs();
  int32_t OpenMpq(const std::string &filename);

  std::string search_dir_;
  WowFileMap_t wow_file_map_;
  MpqArchives_t mpq_arcs_;
};

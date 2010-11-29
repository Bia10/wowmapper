/*  wowmapper - World of Warcraft MAP ParsER
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

// associates filenames inside an archive with its archive
typedef std::map<std::string, mpq_archive_s*> MpqArchiveMap_t;
typedef MpqArchiveMap_t::value_type MpqArchivePair_t;
typedef std::list<mpq_archive_s*> MpqArchiveList_t;
typedef std::list<std::string> FilenameList_t;

/** MpqHandler simplifies MPQ archive handling. */
class MpqHandler {
 public:
  /** Constructor takes the path of the WoW data directory. **/
  MpqHandler( const std::string &data_directory = "." );
  /** Adds a MPQ file to the MPQ handler and returns the number of filenames
      added through the addition of the MPQ file. **/
  size_t addFile( const std::string &filename );
  /** Get a file from our MPQs through its name and return the data. **/
  bool getFile( const std::string &filename, BufferS_t *buffer ) const;
  /** Cleans up and closes all MPQ archives we opened. **/
  void clear();

 private:
  /** Retrieves the list file of a MPQ archive which contains all file names
      in the archive. **/
  libmpq__off_t getListFile( const std::string &filename, mpq_archive_s **mpq_arc,
                             uint8_t **buffer ) const;

  /** Associates filenames inside MPQs with MPQ archives. **/
  MpqArchiveMap_t _mpqArchiveMap;
  /** Hold a list of loaded archives so we can clean it up when we're finished. **/
  MpqArchiveList_t _mpqArchiveList;
  /** MPQ data directory. **/
  std::string _dataDirectory;
};

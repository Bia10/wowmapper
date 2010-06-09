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

#include "mphdchunk_s.h"
#include "mainchunk_s.h"

/*! \brief Used to read WDT files and retrieve ADTs. */
class Wdt_c : public Chunk_c {
 public:
  Wdt_c(Buffer_t *buf, const std::string &name);
  const AdtPos_t& adt_list() const { return list_; }

 private:
  void GetList();

  std::string name_;
  AdtPos_t list_;

  MphdChunk_s mphd_;
  MainChunk_s main_;
};

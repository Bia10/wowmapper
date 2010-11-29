/*  wowmapper - World of Warcraft MAP ParsER
    Copyright (C) 2010 PferdOne aka Flowerew

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
#include "mverchunk.h"

/** Contains ADT flags. **/
struct MphdChunk_s {
  char id[4];
  uint32_t size;
  uint32_t flags;
 private:
  uint32_t pad[7];
};

/** Contains the size in bytes of all AsyncObjects in the file. **/
struct MainChunk_s {
  char id[4];
  uint32_t size;

  /** Indicates wheter or not an ADT is placed at the objects location. **/
  struct AsyncObject_s {
    uint32_t flags;
   private:
    uint32_t pad;
  };
};


typedef std::vector<MainChunk_s::AsyncObject_s> AsyncObjects_t;
typedef std::vector<glm::ivec2> AdtCoords_t;

/** Pass a buffer returned by MpqHandler::getFile to parse a *.wdt file. **/
class Wdt {
 public:
  /** Takes a buffer and parses it. **/
  Wdt( const BufferS_t &buffer );
  /** Returns all coordinates (v/h) which contain ADT tiles. **/
  const AdtCoords_t& getAdtCoords() const;

 private:
  MverChunk_s _mverChunk;
  MphdChunk_s _mphdChunk;
  MainChunk_s _mainChunk;
  AsyncObjects_t _asyncObjects;
  AdtCoords_t _adtCoords;
};
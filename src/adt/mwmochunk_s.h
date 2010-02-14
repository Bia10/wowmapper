#pragma once

#include "../chunk_s.h"

typedef std::vector<std::string> WmoFilenames_t;

/*! @brief MWMO chunk */
struct MwmoChunk_s : Chunk_s {
  WmoFilenames_t filenames;

  MwmoChunk_s(uint32_t offset, void *buffer)
      : Chunk_s(offset, buffer, false) {
    uint32_t buf_addr = reinterpret_cast<uint32_t>(buffer);
    void *src_addr = reinterpret_cast<void*>(buf_addr + offset + sizeof(Chunk_s));

    /* copy all wmo filenames to our temporary buffer */
    char *wmo_filename_buf = new char[size];
    memcpy(wmo_filename_buf, src_addr, size);

    /* push every string we encounter into our filename vector */
    size_t pos = 0;
    while(size > pos) {
      filenames.push_back(wmo_filename_buf+pos);
      pos += strlen(wmo_filename_buf+pos)+1;
    }

    delete [] wmo_filename_buf;
  }
};

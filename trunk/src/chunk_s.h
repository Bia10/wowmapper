#pragma once

#include <string.h>
#include "common.h"

/*! @brief Base chunk structure for all chunks. */
struct Chunk_s {
  char id[4];
  uint32_t size;
};

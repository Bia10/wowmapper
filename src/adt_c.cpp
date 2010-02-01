#include "adt_c.h"
#include <string.h>
#include <stdio.h>

Adt_c::Adt_c(uint8_t *buffer) : raw_data_(buffer) {
  Initialize();
}

Adt_c::~Adt_c() {
  delete [] raw_data_;
}



bool Adt_c::Initialize() {
  /* initialize MHDR chunk */
  int32_t cur_offset = 0xC;
  memcpy(&mhdr_chunk_, (void*)(raw_data_ + cur_offset), sizeof(MhdrChunk_s));

  /* initialize MHDR chunk data */
  cur_offset = (int32_t)mhdr_chunk_.mcin + 0x14;
  mhdr_chunk_.mcin = new McinChunk_s();
  memcpy(mhdr_chunk_.mcin, (void*)(raw_data_ + cur_offset), sizeof(McinChunk_s));

  /* initialize MNCK index */
  for (int i = 0; i < 256; i++) {
    cur_offset = (int32_t)mhdr_chunk_.mcin->mcnk_index[i].mcnk;
    // allocate memory for mcnk: mhdr_chunk_.mcin->mcnk_index[i].mcnk = new McnkChunks_s();
    // memcpy(mhdr_chunk_.mcin->mcnk_index[i].mcnk, (void*)(raw_data_ + cur_offset), sizeof(McnkChunk_s));
    printf("#%d off: 0x%04x size: %d\n", i, mhdr_chunk_.mcin->mcnk_index[i].mcnk,
        mhdr_chunk_.mcin->mcnk_index[i].size);
  }

  return true;
}







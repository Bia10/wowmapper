#include "adt_c.h"

#define MHDR_OFFSET 0xC
#define STEP 2.083333333f       // 100.0f / (3.0f * 16.0f);
#define XZOFFSET 533.333333333f

Adt_c::Adt_c(uint8_t **buffer, MpqHandler_c *mpqHandler /* = NULL */) :
  mhdr_chunk_(MHDR_OFFSET, *buffer) {
  delete [] *buffer;
  *buffer = NULL; // invalidate buffer

  if(mpqHandler == NULL) return;

  LoadWmo(mpqHandler);
}

void Adt_c::LoadWmo(MpqHandler_c *mpqHandler) {
  uint8_t *wmo_buffer = NULL;

  /* begin loading of wmos here */
  for (WmoFilenames_t::iterator wmo_name = mhdr_chunk_.mwmo->filenames.begin();
       wmo_name != mhdr_chunk_.mwmo->filenames.end();
       ++wmo_name) {
    std::cout << "Load WMO: " << *wmo_name << std::endl;

    mpqHandler->LoadFileByName(wmo_name->c_str(), &wmo_buffer);
    wmo_list_.push_back(new Wmo_c(&wmo_buffer, wmo_name->c_str(), mpqHandler));

    wmo_buffer = NULL;
  }
}

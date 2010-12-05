#include "wdt.h"

//------------------------------------------------------------------------------
Wdt::Wdt( const BufferS_t &buffer ) {
  if ( buffer.size() <= 0 ) {
    quitApp( "[WDT] buffer invalid" );
    return;
  }

  // create an istream of our buffer
  std::stringbuf str_buf( buffer );
  std::istream i_str( &str_buf );

  uint32_t chunk_size = 0;
  // read in chunk by chunk
  chunk_size = readChunkHead( i_str, "MVER", (char*)&_mverChunk, sizeof( MverChunk_s ) );
  chunk_size = readChunkHead( i_str, "MPHD", (char*)&_mphdChunk, sizeof( MphdChunk_s ) );
  chunk_size = readChunkHead( i_str, "MAIN", (char*)&_mainChunk, sizeof( MainChunk_s ) );
  
  // I could just say 64*64, but it's there to check that everything went right
  size_t count = _mainChunk.size / sizeof( MainChunk_s::AsyncObject_s );
  size_t num_adts = 0;
  _asyncObjects.resize( count );

  // read flags from buffer
  for ( size_t i = 0; i < count; i++ ) {
    i_str.read( (char*)&_asyncObjects[i], sizeof( MainChunk_s::AsyncObject_s ) );
    
    if ( _asyncObjects[i].flags & 0x1 ) {
      num_adts++;
    }
  }

  _adtCoords.resize( num_adts );
  int i = 0;
  for ( int y = 0; y < 64; y++ ) {
    for ( int x = 0; x < 64; x++ ) {
      // get all tiles with flag & 1
      if ( _asyncObjects[y*64+x].flags & 0x1 ) {
        _adtCoords[i].x = x;
        _adtCoords[i].y = y;
        i++;
      }
    }
  }
}

//------------------------------------------------------------------------------
const AdtCoords_t& Wdt::getAdtCoords() const {
  return _adtCoords;
}
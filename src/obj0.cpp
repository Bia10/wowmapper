#include "obj0.h"

Obj0::Obj0( const BufferS_t &obj_buf ) {
  // create an istream of our buffer
  std::stringbuf str_buf( obj_buf );
  std::istream i_str( &str_buf );

  uint32_t chunk_size = 0;
  // read MVER chunk
  chunk_size = readChunkHead( i_str, "MVER", (char*)&_mverChunk, sizeof( MverChunk_s ) );

  // read MMDX chunk
  chunk_size = readChunkHead( i_str, "MMDX", (char*)&_mmdxChunk );
  if ( _mmdxChunk.size ) {
    _doodadNames.resize( _mmdxChunk.size );
    i_str.read( (char*)&_doodadNames[0], _mmdxChunk.size );
  }

  // read MMID chunk
  chunk_size = readChunkHead( i_str, "MMID", (char*)&_mmidChunk );
  if ( _mmidChunk.size ) {
    _mmdxIndices.resize( _mmidChunk.size / 4 );
    i_str.read( (char*)&_mmdxIndices[0], _mmidChunk.size );
  }

  // read MWMO chunk
  chunk_size = readChunkHead( i_str, "MWMO", (char*)&_mwmoChunk );
  if ( _mwmoChunk.size ) {
    _wmoNames.resize( _mwmoChunk.size );
    i_str.read( (char*)&_wmoNames[0], _mwmoChunk.size );
  }

  // read MMID chunk
  chunk_size = readChunkHead( i_str, "MWID", (char*)&_mwidChunk );
  if ( _mwidChunk.size ) {
    _mwmoIndices.resize( _mwidChunk.size / 4 );
    i_str.read( (char*)&_mwmoIndices[0], _mwidChunk.size );
  }

  // read MDDF chunk
  chunk_size = readChunkHead( i_str, "MDDF", (char*)&_mddfChunk );
  if ( _mddfChunk.size ) {
    _doodadInfo.resize( _mddfChunk.size / sizeof( MddfChunk_s::DoodadInfo_s ) );
    i_str.read( (char*)&_doodadInfo[0], _mddfChunk.size );
  }

  // read MODF chunk
  chunk_size = readChunkHead( i_str, "MODF", (char*)&_modfChunk );
  if ( _modfChunk.size ) {
    _wmoInfo.resize( _modfChunk.size / sizeof( ModfChunk_s::WmoInfo_s ) );
    i_str.read( (char*)&_wmoInfo[0], _modfChunk.size );
  }

  parseObjectReferences( i_str );
}

//------------------------------------------------------------------------------
void Obj0::parseObjectReferences( std::istream &i_str ) {
  // the same amount of MCNKs as in the usual *.adt files
  _objectRefs.resize( 256 );
  size_t zero_mem_size = sizeof( ObjMcnkChunk_s ) + sizeof( McrdChunk_s ) +
                         sizeof( McrwChunk_s );

  // get all object reference MCNKs
  for ( int i = 0; i < 256; i++ ) {
    memset( &_objectRefs[i], 0, zero_mem_size ); // set all chunks to zero

    // there is always a MCNK chunk first
    ObjMcnkChunk_s &mcnk_chunk = _objectRefs[0].mcnkChunk;
    i_str.read( (char*)&mcnk_chunk, sizeof( ObjMcnkChunk_s ) );
    size_t data_off = i_str.tellg();

    // I believe there is no MCRW without MCRD chunks, but MCRD is always first
    if ( mcnk_chunk.size ) {
      McrdChunk_s &mcrd_chunk = _objectRefs[i].mcrdChunk;
      i_str.read( (char*)&mcrd_chunk, sizeof( McrdChunk_s ) );
      
      // read doodad indices
      _objectRefs[i].doodadIndices.resize( mcrd_chunk.size / 4 );
      i_str.read( (char*)&_objectRefs[i].doodadIndices[0], mcrd_chunk.size );

      // check if there's a MCRW chunk left to read
      if ( (data_off + mcnk_chunk.size) > i_str.tellg() ) {
        McrwChunk_s &mcrw_chunk = _objectRefs[i].mcrwChunk;
        i_str.read( (char*)&mcrw_chunk, sizeof( McrwChunk_s ) );

        // read WMO indices
        _objectRefs[i].wmoIndices.resize( mcrw_chunk.size / 4 );
        i_str.read( (char*)&_objectRefs[i].wmoIndices[0], mcrw_chunk.size );
      }
    }
  }
}

//------------------------------------------------------------------------------
const ObjectReferences_t& Obj0::getObjectRefs() const {
  return _objectRefs;
}

//------------------------------------------------------------------------------
const WmoInformations_t& Obj0::wmoInfo() const {
  return _wmoInfo;
}

//------------------------------------------------------------------------------
void Obj0::getDoodad( uint32_t index, Doodad_s *doodad ) const {
  if ( index+1 > _doodadInfo.size() ) return;

  const MddfChunk_s::DoodadInfo_s &doodad_info = _doodadInfo[index];
  uint32_t mmid = doodad_info.id;  // index into the doodad indices
  uint32_t name_off = _mmdxIndices[mmid]; // name offset 

  std::stringbuf str_buf( _doodadNames );
  std::istream i_str( &str_buf );

  char dn_buf[256];                   // buffer to hold the name returned by getline
  i_str.seekg( name_off );            // position stream at the right position
  i_str.getline( dn_buf, 256, '\0' );
  std::string doodad_name( dn_buf );  // I like strings :)

  // return data
  memcpy( &doodad->info, &doodad_info, sizeof( MddfChunk_s::DoodadInfo_s ) );
  doodad->name = doodad_name;
}

//------------------------------------------------------------------------------
void Obj0::getWmo( uint32_t index, Wmo_s *wmo ) const {
  if ( index+1 > _wmoInfo.size() ) return;

  // pretty much the same as in getDoodad()
  const ModfChunk_s::WmoInfo_s &wmo_info = _wmoInfo[index];
  uint32_t mwid = wmo_info.id;         
  uint32_t name_off = _mwmoIndices[mwid]; 

  std::stringbuf str_buf( _wmoNames );
  std::istream i_str( &str_buf );

  char dn_buf[256];                   
  i_str.seekg( name_off );            
  i_str.getline( dn_buf, 256, '\0' );
  std::string wmo_name( dn_buf );

  // return data
  memcpy( &wmo->info, &wmo_info, sizeof( ModfChunk_s::WmoInfo_s ) );
  wmo->name = wmo_name;
}
#include "wmogroup.h"

//------------------------------------------------------------------------------
WmoGroup::WmoGroup( BufferS_t &grp_buf ) {
  // create an istream of our buffer
  std::stringbuf str_buf( grp_buf );
  std::istream i_str( &str_buf );

  // read in chunk by chunk
  i_str.read( (char*)&_mverChunk, sizeof( MverChunk_s ) );
  i_str.read( (char*)&_mogpChunk, sizeof( MogpChunk_s ) );

  // read MOPY chunk: material infos
  i_str.read( (char*)&_mopyChunk, CHUNK_DATA );
  _mopyChunk.infos.resize( _mopyChunk.size / sizeof( MopyChunk_s::MaterialInformation_s ) );
  i_str.read( (char*)&_mopyChunk.infos[0], _mopyChunk.size );

  // read MOVI chunk: vertex indices
  i_str.read( (char*)&_moviChunk, CHUNK_DATA );
  _moviChunk.indices.resize( _moviChunk.size / sizeof( uint16_t ) );
  i_str.read( (char*)&_moviChunk.indices[0], _moviChunk.size );

  // read MOVT chunk: vertices
  i_str.read( (char*)&_movtChunk, CHUNK_DATA );
  _movtChunk.vertices.resize( _movtChunk.size / sizeof( glm::vec3 ) );
  i_str.read( (char*)&_movtChunk.vertices[0], _movtChunk.size );

  // read MONR chunk: normals
  i_str.read( (char*)&_monrChunk, CHUNK_DATA );
  _monrChunk.normals.resize( _monrChunk.size / sizeof( glm::vec3 ) );
  i_str.read( (char*)&_monrChunk.normals[0], _monrChunk.size );

  // read MOTV chunk: texture coordinates
  i_str.read( (char*)&_motvChunk, CHUNK_DATA );
  _motvChunk.texCoords.resize( _motvChunk.size / sizeof( glm::vec2 ) );
  i_str.read( (char*)&_motvChunk.texCoords[0], _motvChunk.size );
}

//------------------------------------------------------------------------------
const MopyChunk_s& WmoGroup::getMopyChunk() const {
  return _mopyChunk;
}

//------------------------------------------------------------------------------
const MoviChunk_s& WmoGroup::getMoviChunk() const {
  return _moviChunk;
}

//------------------------------------------------------------------------------
const MovtChunk_s& WmoGroup::getMovtChunk() const {
  return _movtChunk;
}

//------------------------------------------------------------------------------
const MonrChunk_s& WmoGroup::getMonrChunk() const {
  return _monrChunk;
}

//------------------------------------------------------------------------------
const MotvChunk_s& WmoGroup::getMotvChunk() const {
  return _motvChunk;
}
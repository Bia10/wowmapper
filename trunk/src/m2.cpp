#include "m2.h"

//------------------------------------------------------------------------------
M2::M2( const BufferS_t &m2_buf ) {
  // create an istream of our buffer
  std::stringbuf str_buf( m2_buf );
  std::istream i_str( &str_buf );

  // read in chunk by chunk
  i_str.read( (char*)&_m2Header, sizeof( M2Header_s ) );

  // get M2 model name
  i_str.seekg( _m2Header.nameOff );
  _m2name.resize( _m2Header.nameLength );
  i_str.getline( &_m2name[0], _m2name.size(), '\0' );

  if ( _m2Header.numVertices ) {
    _vertices.resize( _m2Header.numVertices );
    i_str.seekg( _m2Header.verticesOff );
    i_str.read( (char*)&_vertices[0], _vertices.size() * sizeof( glm::vec3 ) );
  }

  // get bounding triangles
  if ( _m2Header.numBoundingTriangles ) {
    _boundingIndices.resize( _m2Header.numBoundingTriangles );
    i_str.seekg( _m2Header.boundingTriangleOff );
    i_str.read( (char*)&_boundingIndices[0],
                _boundingIndices.size() * sizeof( uint16_t ) );
  }

  // get bounding vertices
  if ( _m2Header.numBoundingVertices ) {
    _boundingVertices.resize( _m2Header.numBoundingVertices );
    i_str.seekg( _m2Header.boundingVerticesOff );
    i_str.read( (char*)&_boundingVertices[0],
                _boundingVertices.size() * sizeof( glm::vec3 ) );
  }

  // get bounding normals
  if ( _m2Header.numBoundingNormals ) {
    _boundingNormals.resize( _m2Header.numBoundingVertices );
    i_str.seekg( _m2Header.boundingNormalsOff );
    i_str.read( (char*)&_boundingNormals[0],
                _boundingNormals.size() * sizeof( glm::vec3 ) );
  }
}

//------------------------------------------------------------------------------
void M2::getBoundingVertices( Vertices_t *vertices ) {
  if ( !_boundingVertices.size() ) return;

  vertices->resize( _boundingVertices.size() );
  std::copy( _boundingVertices.begin(), _boundingVertices.end(), vertices->begin() );

  /*static const float map_offset = 17066.0f + (2/3.0f);
  static const glm::vec3 map_pos( map_offset, 0, map_offset );

  // copy untransformed vertices to output vector
  vertices->resize( _boundingVertices.size() );
  std::copy( _boundingVertices.begin(), _boundingVertices.end(), vertices->begin() );

  // create rotation matrix
  const glm::vec3 &rotation = doodad.info.rot;
  glm::mat4 rot_mtx;
  rot_mtx = glm::rotate( rot_mtx, rotation.y - 90, glm::vec3(0, 1, 0) );
  rot_mtx = glm::rotate( rot_mtx, -rotation.x,     glm::vec3(0, 0, 1) );
  rot_mtx = glm::rotate( rot_mtx, rotation.z - 90, glm::vec3(1, 0, 0) );

  float scale = doodad.info.scale / 1024.0f;

  // transform vertices
  for ( Vertices_t::iterator vertex = vertices->begin();
        vertex != vertices->end();
        ++vertex ) {
    glm::vec4 v4( *vertex, 0 );
    v4 = scale * rot_mtx * v4;
    *vertex = glm::vec3( v4 );
    *vertex += doodad.info.pos - map_pos;*/ 
    //*vertex += doodad.info.pos;
    /*vertex->x -= doodad.info.pos.y; // +map_offset
    vertex->y += doodad.info.pos.z;
    vertex->z -= doodad.info.pos.x; // +map_offset
  }*/
}

//------------------------------------------------------------------------------
void M2::getBoundingIndices( Indices32_t *indices ) {
  indices->resize( _boundingIndices.size() );
  std::copy( _boundingIndices.begin(), _boundingIndices.end(), indices->begin() );
}

//------------------------------------------------------------------------------
void M2::getBoundingNormals( Normals_t *normals ) {
  normals->resize( _boundingNormals.size() );
  std::copy( _boundingNormals.begin(), _boundingNormals.end(), normals->begin() );
}
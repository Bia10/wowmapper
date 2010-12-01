#include "adt.h"

Indices32_t Terrain_s::generatedIndices;
Vertices_t Terrain_s::generatedVertices;

//------------------------------------------------------------------------------
Adt::Adt( const BufferS_t &adt_buf ) {
  // pre-generate indices and vertices
  generateIndices();
  generateVertices();

  // create an istream of our buffer
  std::stringbuf str_buf( adt_buf );
  std::istream i_str( &str_buf );

  uint32_t chunk_size = 0;

  // read in chunk by chunk
  chunk_size = readChunkHead( i_str, "MVER", (char*)&_mverChunk, sizeof( MverChunk_s ) );
  chunk_size = readChunkHead( i_str, "MHDR", (char*)&_mhdrChunk, sizeof( MhdrChunk_s ) );

  parseMh2oChunk( i_str );

  // prepare mcnk chunks and height map for data
  _mcnkChunks.resize( 256 );
  _adtTerrain.resize( 256 );

  // get vertices for terrain tile
  for ( int i = 0; i < 256; i++ ) {
    parseMcnkChunk( i_str, &_mcnkChunks[i], &_adtTerrain[i] );

    _adtTerrain[i].areaId = _mcnkChunks[i].areaId;
    _adtTerrain[i].position = _mcnkChunks[i].position;

    // create final buffers
    getGeometry( &_adtTerrain[i] );
    markHoles( _mcnkChunks[i].holes, &_adtTerrain[i] );
    removeGeometry( &_adtTerrain[i] );
  }
  
  size_t pos = i_str.tellg();
}

//------------------------------------------------------------------------------
Adt::~Adt() {
  for ( Mh2oChunk_s::Headers_t::iterator head_iter = _mh2oChunk.headers.begin();
        head_iter != _mh2oChunk.headers.end();
        ++head_iter ) {
    // cleanup information data
    Mh2oChunk_s::Header_s::Informations_t *infos = head_iter->infos;
    if ( infos ) {
      for ( Mh2oChunk_s::Header_s::Informations_t::iterator info_iter = infos->begin();
            info_iter != infos->end();
            ++info_iter ) {
        delete info_iter->mask2;
        delete info_iter->heightMap;
      }
    }

    delete head_iter->infos;
    delete head_iter->renderMap;
  }
}

//------------------------------------------------------------------------------
const AdtTerrain_t& Adt::getTerrain() const {
  return _adtTerrain;
}

//------------------------------------------------------------------------------
bool Adt::parseMh2oChunk( std::istream &i_str ) {
  if ( !_mhdrChunk.mh2oOff ) return false;

  uint32_t pos = i_str.tellg();
  uint32_t data_offset = pos + CHUNK_DATA;
  size_t chunk_size = readChunkHead( i_str, "MH2O", (char*)&_mh2oChunk );

  // read MH2O headers
  Mh2oChunk_s::Headers_t &headers = _mh2oChunk.headers;
  headers.resize( 256 );
  for ( int i = 0; i < 256; i++ ) {
    i_str.read( (char*)&headers[i], sizeof( Mh2oChunk_s::Header_s ) );
  }
  
  // get information
  for ( int i = 0; i < 256; i++ ) {
    // get header values
    Mh2oChunk_s::Header_s &header = headers[i];
    uint32_t render_offset = (uint32_t)header.renderMap;
    uint32_t info_offset = (uint32_t)header.infos;
    uint32_t num_layers = (uint32_t)header.numLayers;
    
    // set position to read the render map
    header.renderMap = new uint64_t;
    i_str.seekg( data_offset + render_offset );
    i_str.read( (char*)header.renderMap, sizeof( uint64_t ) );
    
    // create layers
    if ( info_offset && num_layers ) {
      header.infos = new Mh2oChunk_s::Header_s::Informations_t( num_layers );
      // set seek position to information offset
      i_str.seekg( data_offset + info_offset );
    }

    // read information layers
    for ( uint32_t j = 0; j < header.numLayers; j++ ) {
      Mh2oChunk_s::Header_s::Information_s &info = (*header.infos)[j];
      i_str.read( (char*)&info, sizeof( Mh2oChunk_s::Header_s::Information_s ) );

      // get mask and heightmap
      uint32_t mask2_offset = (uint32_t)info.mask2;
      uint32_t height_map_offset = (uint32_t)info.heightMap;

      // read mask
      if ( mask2_offset && (info.height > 0) ) {
        
        info.mask2 = new Indices8_t( info.height );
        i_str.seekg( data_offset + mask2_offset );
        i_str.read( (char*)&(*info.mask2)[0], sizeof( uint8_t ) * info.height );
      } else {
        info.mask2 = NULL;
      }

      // read height map
      if ( height_map_offset && (info.width*info.height > 0) ) {
        info.heightMap = new HeightMap_t( info.width * info.height );
        i_str.seekg( data_offset + height_map_offset );
        i_str.read( (char*)&(*info.heightMap)[0],
                    sizeof( float ) * info.width * info.height );
      } else {
        info.heightMap = NULL;
      }
    }
  }

  // set seek position to end of MH2O chunk
  i_str.seekg( data_offset + _mh2oChunk.size );

  return true;
}

//------------------------------------------------------------------------------
void Adt::parseMcnkChunk( std::istream &i_str, McnkChunk_s *mcnk_chunk,
                          Terrain_s *terrain ) {
  uint32_t mcnk_off = i_str.tellg();

  // read MCNK chunk
  i_str.read( (char*)mcnk_chunk, sizeof( McnkChunk_s ) );
 
  // set stream position to MCVT chunk
  McvtChunk_s mcvt_chunk;
  i_str.seekg( mcnk_off + mcnk_chunk->mcvtOff, std::ios::beg );
  i_str.read( (char*)&mcvt_chunk, sizeof( McvtChunk_s ) );

  // read all height values
  int num_heights = mcvt_chunk.size / sizeof( float );
  HeightMap_t &height_map = terrain->heightMap;
  height_map.resize( num_heights );
  for ( int i = 0; i < num_heights; i++ ) {
    i_str.read( (char*)&height_map[i], sizeof( float ) );
  }


  // set stream position to MCNR chunk
  McnrChunk_s mcnr_chunk;
  i_str.seekg( mcnk_off + mcnk_chunk->mcnrOff, std::ios::beg );
  i_str.read( (char*)&mcnr_chunk, sizeof( McnrChunk_s ) );

  // read all normal values
  //int num_normals = mcnr_chunk.size / sizeof( char ); // size is wrong
  Normals_t &normals = terrain->normals;
  normals.resize( num_heights );
  char normal = 0;
  for ( int i = 0; i < num_heights; i++ ) {
    i_str.read( &normal, sizeof( char ) );
    normals[i].x = normal * 0.00787401574803149606299212598425f;
    i_str.read( &normal, sizeof( char ) );
    normals[i].z = normal * 0.00787401574803149606299212598425f;
    i_str.read( &normal, sizeof( char ) );
    normals[i].y = normal * 0.00787401574803149606299212598425f;
  }

  // set stream position behind
  i_str.seekg( mcnk_off + 0x8 + mcnk_chunk->size, std::ios::beg );
}

//------------------------------------------------------------------------------
void Adt::getGeometry( Terrain_s *terrain ) const {
  // copy indices
  terrain->indices.resize( 768 );
  std::copy( Terrain_s::generatedIndices.begin(), Terrain_s::generatedIndices.end(),
             terrain->indices.begin() );
  // copy vertices
  terrain->vertices.resize( 145 );
  std::copy( Terrain_s::generatedVertices.begin(), Terrain_s::generatedVertices.end(),
             terrain->vertices.begin() );

  // convert to a usable vertex format with y up instead of z
  // and assign height information
  for ( int i = 0; i < 145; i++ ) {
    terrain->vertices[i].x -= terrain->position.y;
    terrain->vertices[i].y = terrain->heightMap[i] + terrain->position.z;
    terrain->vertices[i].z -= terrain->position.x;
  }
}

//------------------------------------------------------------------------------
void Adt::generateIndices() {
  Indices32_t &indices = Terrain_s::generatedIndices;
  // check if indices have already been generated
  if ( indices.size() ) return;
  // resize and fill indices
  indices.resize( 768 );

  for ( int y = 0; y < 8; y++ ) {
    for ( int x = 0; x < 8; x++ ) {
      uint32_t i = y * 8 * 12 + x * 12;

      // vertex 0
      indices[i + 0] = y * 17 + x;      // top left
      indices[i + 2] = y * 17 + x + 1;  // top right
      indices[i + 1] = y * 17 + x + 9;  // center

      // vertex 1
      indices[i + 3] = y * 17 + x + 1;  // top right
      indices[i + 5] = y * 17 + x + 18; // bottom right
      indices[i + 4] = y * 17 + x + 9;  // center

      // vertex 2
      indices[i + 6] = y * 17 + x + 18; // bottom right
      indices[i + 8] = y * 17 + x + 17; // bottom left
      indices[i + 7] = y * 17 + x + 9;  // center

      // vertex 3
      indices[i + 9] = y * 17 + x + 17; // bottom left
      indices[i + 11] = y * 17 + x;     // top left
      indices[i + 10] = y * 17 + x + 9; // center
    }
  }
}

//------------------------------------------------------------------------------
void Adt::generateVertices() {
  Vertices_t &vertices = Terrain_s::generatedVertices;
  // check if vertices have already been generated
  if ( vertices.size() ) return;
  // resize and fill indices
  vertices.resize( 145 );

  const float step = 100.0f / (3.0f * 16.0f);

  for ( int i = 0; i < 145; i++ ) {
    uint32_t row_idx = i / 17;
    uint32_t col_idx = i % 17;

    bool isRow9x9 = col_idx < 9;
    if ( isRow9x9 ) { // if it's a row of the outer 9x9 vector ...
      vertices[i].x = col_idx * step * 2;
      vertices[i].z = row_idx * step * 2;
    } else { // if it's a row of the inner 8x8 vector ...
      vertices[i].x = (col_idx - 9) * step * 2 + step;
      vertices[i].z = row_idx * step * 2 + step;
    }
    
    vertices[i].y = 0;
  }
}

//------------------------------------------------------------------------------
void Adt::markHoles( uint32_t holes, Terrain_s *terrain ) const {
  for ( int hole_y = 0; hole_y < 8; hole_y++ ) {
    for ( int hole_x = 0; hole_x < 8; hole_x++ ) {
      // if hole is found mark the corresponding terrain indices with unsigned int max
      // a bit in the hole mask always covers 1 square == 4 triangles == 12 indices
      if ( holes & ( 1 << ( hole_y/2 * 4 + hole_x/2 ) ) ) {
        int curent_hole_index = hole_y*8*12 + hole_x*12;
        for ( int i = 0; i < 12; i++ ) {
          terrain->indices[ curent_hole_index + i ] = -1;
        }
      }
    }
  }
}

//------------------------------------------------------------------------------
void Adt::removeGeometry( Terrain_s *terrain ) const {
  Indices32_t &indices = terrain->indices;
  Vertices_t &vertices = terrain->vertices;
  Normals_t &normals = terrain->normals;

  // indices remapped
  Indices32_t remapped_indices( indices.size(), -1 ); 

  // new indices, vertices and normals
  Indices32_t new_indices; new_indices.reserve( indices.size() );
  Vertices_t new_vertices; new_indices.reserve( vertices.size() );
  Normals_t new_normals; new_normals.reserve( normals.size() );

  uint32_t new_index = 0;
  uint32_t marked_index = 0;

  for ( uint32_t i = 0; i < indices.size(); i++ ) {
    marked_index = indices[i];
    // check if index is valid
    if ( marked_index != -1 ) {
      // check if we already have a new index value for the old one
      if ( remapped_indices[marked_index] == -1 ) {
        // we have a new index so map the new value and insert vtx and norms
        remapped_indices[marked_index] = new_index;
        new_vertices.push_back( vertices[marked_index] );
        new_normals.push_back( normals[marked_index] );
        new_index++;
      }
      // push mapped index value to new index array
      new_indices.push_back( remapped_indices[marked_index] );
    }
  }

  // swap new buffers with old ones
  indices.swap( new_indices );
  vertices.swap( new_vertices );
  normals.swap( new_normals );
}
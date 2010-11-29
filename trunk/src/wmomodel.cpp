#include "wmomodel.h"

//------------------------------------------------------------------------------
WmoModel::WmoModel( const BufferS_t &wmo_buf ) {
  // create an istream of our buffer
  std::stringbuf str_buf( wmo_buf );
  std::istream i_str( &str_buf );

  // read in chunk by chunk
  i_str.read( (char*)&_mverChunk, sizeof( MverChunk_s ) );
  i_str.read( (char*)&_mohdChunk, sizeof( MohdChunk_s ) );
  
  // read MOTX filenames
  i_str.read( (char*)&_motxChunk, CHUNK_DATA );
  _motxChunk.textureNames.resize( _motxChunk.size );
  i_str.read( &_motxChunk.textureNames[0], _motxChunk.size );

  // read MOTM chunk and material structs for each BLP inside
  i_str.read( (char*)&_momtChunk, CHUNK_DATA );
  _momtChunk.materials.resize( _momtChunk.size / sizeof( MomtChunk_s::Materials_s ) );
  i_str.read( (char*)&_momtChunk.materials[0], _momtChunk.size );

  // read MOGN group names
  i_str.read( (char*)&_mognChunk, CHUNK_DATA );
  _mognChunk.groupNames.resize( _mognChunk.size );
  i_str.read( &_mognChunk.groupNames[0], _mognChunk.size );  

  // read MOGI chunk and its group informations
  i_str.read( (char*)&_mogiChunk, CHUNK_DATA );
  _mogiChunk.infos.resize( _mogiChunk.size / sizeof( MogiChunk_s::GroupInformation_s ) );
  i_str.read( (char*)&_mogiChunk.infos[0], _mogiChunk.size );

  // read MOSB chunk: sky box
  i_str.read( (char*)&_mosbChunk, sizeof( MosbChunk_s ) );

  // read MOPV chunk: portal vertices
  i_str.read( (char*)&_mopvChunk, CHUNK_DATA );
  i_str.seekg( _mopvChunk.size + i_str.tellg() );

  // had problems :( 58 and a half portal vertices, result: memory corruption, so skip it
  /*if ( _mopvChunk.size ) {
    size_t v_size = sizeof( MopvChunk_s::PortalVertices_s );
    std::cout << "MOPV size: " << (float( _mopvChunk.size ) / v_size) << std::endl;
    _mopvChunk.vertices.resize( _mopvChunk.size / v_size );
    i_str.read( (char*)&_mopvChunk.vertices[0], _mopvChunk.size );
  }*/

  // read MOPT chunk: portal informations
  i_str.read( (char*)&_moptChunk, CHUNK_DATA );
  if ( _moptChunk.size ) {
    _moptChunk.infos.resize( _moptChunk.size / sizeof( MoptChunk_s::PortalInformation_s ) );
    i_str.read( (char*)&_moptChunk.infos[0], _moptChunk.size );
  }

  // read MOPR chunk: portal refererences
  i_str.read( (char*)&_moprChunk, CHUNK_DATA );
  if ( _moprChunk.size ) {
    _moprChunk.references.resize( _moprChunk.size / sizeof( MoprChunk_s::PortalReference_s ) );
    i_str.read( (char*)&_moprChunk.references[0], _moprChunk.size );
  }

  // read MOVV chunk
  i_str.read( (char*)&_movvChunk, CHUNK_DATA );
  if ( _movvChunk.size ) {
    _movvChunk.content.resize( _movvChunk.size );
    i_str.read( (char*)&_movvChunk.content[0], _movvChunk.size );
  }

  // read MOVB chunk
  i_str.read( (char*)&_movbChunk, CHUNK_DATA );

  // read MOLT chunk: lighting information
  i_str.read( (char*)&_moltChunk, CHUNK_DATA );
  if ( _moltChunk.size ) {
    _moltChunk.infos.resize( _moltChunk.size / sizeof( MoltChunk_s::LightInformation_s ) );
    i_str.read( (char*)&_moltChunk.infos[0], _moltChunk.size );
  }

  // read MODS chunk: doodad sets
  i_str.read( (char*)&_modsChunk, CHUNK_DATA );
  _modsChunk.doodadSets.resize( _modsChunk.size / sizeof( ModsChunk_s::DoodadSet_s ) );
  i_str.read( (char*)&_modsChunk.doodadSets[0], _modsChunk.size );

  // read MODN chunk: doodad names
  i_str.read( (char*)&_modnChunk, CHUNK_DATA );
  _modnChunk.doodadNames.resize( _modnChunk.size );
  i_str.read( (char*)&_modnChunk.doodadNames[0], _modnChunk.size );

  // read MODD chunk: doodad informations
  i_str.read( (char*)&_moddChunk, CHUNK_DATA );
  if ( _moddChunk.size ) {
    _moddChunk.infos.resize( _moddChunk.size / sizeof( ModdChunk_s::DoodadInformation_s ) );
    i_str.read( (char*)&_moddChunk.infos[0], _moddChunk.size );
  }
}

//------------------------------------------------------------------------------
WmoModel::~WmoModel() {
  for ( WmoGroups_t::iterator iter = _wmoGroups.begin();
        iter != _wmoGroups.end();
        ++iter ) {
    delete *iter;
  }

  _wmoGroups.clear();
}

//------------------------------------------------------------------------------
void WmoModel::loadGroups( const std::string wmo_name, MpqHandler &mpq_h ) {
  // world\maps\some\building.wmo -> world\maps\some\building
  std::string group_name = wmo_name.substr( 0, wmo_name.size() - 4 );
  char group_index[5];

  for ( int i = 0; i < _mohdChunk.numGroups; i++ ) {
    sprintf_s( group_index, 5, "_%03d", i );
    // "world\maps\some\building" + "_00i" + ".wmo"
    std::string group_filename = group_name + group_index + std::string( ".wmo" );
    
    // load file and push to vector
    BufferS_t grp_buf;
    mpq_h.getFile( group_filename, &grp_buf );
    _wmoGroups.push_back( new WmoGroup( grp_buf ) );
  }
}

//------------------------------------------------------------------------------
void WmoModel::getIndices( Indices32_t *indices, uint32_t filter, uint32_t off ) const {
  uint32_t index_off = 0;
  for ( WmoGroups_t::const_iterator iter = _wmoGroups.begin();
        iter != _wmoGroups.end();
        ++iter ) {
    // get group indices
    const Indices16_t &grp_indices = (*iter)->getMoviChunk().indices;
    const Vertices_t &grp_vertices = (*iter)->getMovtChunk().vertices;
    Indices32_t increment( grp_indices.begin(), grp_indices.end() );

    // increment indices
    std::vector<uint32_t> add( grp_indices.size(), off + index_off );
    std::transform( increment.begin(), increment.end(), add.begin(),
                    increment.begin(), std::plus<uint32_t>() );

    // filter vertices by triangle material info
    size_t num_triangles = grp_indices.size() / 3;
    for ( int i = 0; i < num_triangles; i++ ) {
      if ( !((*iter)->getMopyChunk().infos[i].flags & filter) ) {
        increment[i*3+0] = -1;
        increment[i*3+1] = -1;
        increment[i*3+2] = -1;
      }
    }

    // add new indices to destination
    indices->insert( indices->end(), increment.begin(), increment.end() );
    index_off += grp_vertices.size();
  }
}

//------------------------------------------------------------------------------
void WmoModel::getVertices( Vertices_t *vertices ) const {
  for ( WmoGroups_t::const_iterator iter = _wmoGroups.begin();
        iter != _wmoGroups.end();
        ++iter ) {
    // get vertices from group file
    const Vertices_t &grp_vertices = (*iter)->getMovtChunk().vertices;
    vertices->insert( vertices->end(), grp_vertices.begin(), grp_vertices.end() );
  }
}

//------------------------------------------------------------------------------
void WmoModel::getNormals( Normals_t *normals ) const {
  for ( WmoGroups_t::const_iterator iter = _wmoGroups.begin();
        iter != _wmoGroups.end();
        ++iter ) {
    // get normals from group file
    const Normals_t &grp_normals = (*iter)->getMonrChunk().normals;
    normals->insert( normals->end(), grp_normals.begin(), grp_normals.end() );
  }
}
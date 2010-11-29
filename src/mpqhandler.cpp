#include "mpqhandler.h"

//------------------------------------------------------------------------------
MpqHandler::MpqHandler(  const std::string &data_directory )
    : _dataDirectory( data_directory ) {

}

//------------------------------------------------------------------------------
size_t MpqHandler::addFile( const std::string &filename ) {
  mpq_archive_s *mpq_arc = NULL;
  uint8_t *buffer = NULL;
  char *context = NULL;
  
  // get file list from MPQ archive
  if ( getListFile( filename, &mpq_arc, &buffer ) < 0 ) return false;

  size_t num_files = _mpqArchiveMap.size();
  std::string add_fn;
  // get all filenames from the buffer
  for( const char *fn = strtok_s( (char*)buffer, "\r\n", &context );
        fn != NULL;
        fn = strtok_s( NULL, "\r\n", &context ) ) {
    // transform filename to lower case
    add_fn = fn;
    std::transform( add_fn.begin(), add_fn.end(), add_fn.begin(), ::tolower );
    // insert filename in map and associate mpq archive with it so we can get the
    // correct version of the file
    _mpqArchiveMap.insert( MpqArchivePair_t( add_fn, mpq_arc ) );
  }

  delete [] buffer;
  
  // return the number of files we've added
  return _mpqArchiveMap.size() - num_files;
}

//------------------------------------------------------------------------------
libmpq__off_t MpqHandler::getListFile( const std::string &filename,
                                       mpq_archive_s **mpq_arc,
                                       uint8_t **buffer ) const {
  int32_t err = 0;
  std::string mpq_file = _dataDirectory + std::string( "\\" ) + filename;

  try {
    // open mpq archive
    err = libmpq__archive_open( mpq_arc, mpq_file.c_str(), -1 );
    if ( err != LIBMPQ_SUCCESS ) {
      throw( std::string( "libmpq__archive_open failed" ) );
    }

    // get file number of the list file which contains all names in the archive
    uint32_t file_num = 0;
    err = libmpq__file_number( *mpq_arc, LIBMPQ_LISTFILE_NAME, &file_num );
    if ( err != LIBMPQ_SUCCESS ) {
      throw( std::string( "libmpq__file_number failed" ) );
    }

    // read file from archive...get file size and reserve space
    libmpq__off_t file_size;
    err = libmpq__file_unpacked_size( *mpq_arc, file_num, &file_size );
    if ( err != LIBMPQ_SUCCESS ) {
      throw( std::string( "libmpq__file_unpacked_size failed" ) );
    }

    // read file from archive to buffer
    *buffer = new uint8_t[file_size];
    err = libmpq__file_read( *mpq_arc, file_num, *buffer, file_size, NULL );
    if ( err != LIBMPQ_SUCCESS ) {
      throw( std::string( "libmpq__file_read failed" ) );
    }

    return file_size;
  } catch( std::string err_msg ) {
    // if an archive has been opened close it
    if ( *mpq_arc ) {
      libmpq__archive_close( *mpq_arc );
    }

    delete [] *buffer;

    // quit application
    std::stringstream ss;
    ss << "Cannot open " << mpq_file << " " << err_msg;
    quitApp( ss.str() );
  }

  return -1;
}

//------------------------------------------------------------------------------
void MpqHandler::clear() {
  for ( MpqArchiveList_t::iterator mpq_arc = _mpqArchiveList.begin();
        mpq_arc != _mpqArchiveList.end();
        ++mpq_arc ) {
    libmpq__archive_close( *mpq_arc );
  }
}

//------------------------------------------------------------------------------
bool MpqHandler::getFile( const std::string &filename, BufferS_t *buffer ) const {
  libmpq__off_t unpacked = 0;
  libmpq__off_t bytes = 0;
  uint32_t file_num = 0;
  int32_t err = 0;

  // convert filename to lower case string
  std::string fn_lc( filename );
  std::transform( fn_lc.begin(), fn_lc.end(), fn_lc.begin(), ::tolower );

  // search for filename in our map
  MpqArchiveMap_t::const_iterator f = _mpqArchiveMap.find( fn_lc );
  if ( f != _mpqArchiveMap.end() ) {
    mpq_archive_s *mpq_arc = f->second;
    // get file number from mpq archive
    err = libmpq__file_number( mpq_arc, filename.c_str(), &file_num );
    err = libmpq__file_unpacked_size( mpq_arc, file_num, &unpacked );

    // resize buffer and load file into that buffer
    buffer->resize( unpacked );
    uint8_t *buf_p = (uint8_t*)&(*buffer)[0];
    err = libmpq__file_read( mpq_arc, file_num, buf_p, unpacked, &bytes );
  }

  return bytes > 0;
}

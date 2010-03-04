#pragma once

#include "common.h"
class Chunk_s;

/*! \brief  Chunk reader class provides easy to use functions to read chunks
 *          from wow file buffers usually provided by libmpq. */
class ChunkReader_c {
 public:
  ChunkReader_c() {}
  ChunkReader_c(const uint8_t *buffer, int64_t length);
  virtual ~ChunkReader_c() {}

  void AssignBuffer(const uint8_t *buffer, int64_t length);

  /*! \brief  Retrieves a chunk by its ID (a 4 char value).
   *  \param  id A 4 character value: char id[] = { 'C', 'U', 'N', 'K' }.
   *  \param  chunk Chunk which gets the requested chunk assigned.
   *  \return Offset of the chunk in the buffer, -1 on error. */
  size_t GetChunkById(const char *id, Chunk_s *chunk) const;

  /*! \brief  Retrieves a chunk by its ID and the following chunk ID.
   *  \param  idStart A 4 character value: char id[] = { 'C', 'U', 'N', 'K' }.
   *  \param  idEnd A 4 character value: char id[] = { 'C', 'U', 'N', 'K' }.
   *  \param  chunk Chunk which gets the requested chunk assigned.
   *  \return Offset of the chunk in the buffer, -1 on error. */
  size_t GetChunkById(const char *idStart, const char *idEnd, Chunk_s *chunk) const;

  /*! \brief  Retrieves a chunk by its offset in a buffer.
   *  \param  offset Offset of the chunk in the buffer.
   *  \param  chunk Chunk which gets the requested chunk assigned.
   *  \return Returns the size of the chunk. */
  size_t GetChunkByOffset(size_t offset, Chunk_s *chunk) const;

  Buffer_t& raw_buffer();

 protected:
  size_t GetBufferIndexOfChunk(const char *id) const;
  size_t GetChunkDataSize(size_t chunkIndex) const;

  Buffer_t raw_buffer_;
  std::string buffer_string_;
};

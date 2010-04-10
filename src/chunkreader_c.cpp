#include "chunkreader_c.h"
#include "chunk_s.h"

const uint32_t CHUNK_SIZE_OFFSET = 0x4;
const uint32_t CHUNK_SIZE = 0x8;

ChunkReader_c::ChunkReader_c(const uint8_t *buffer, int64_t length)
    : raw_buffer_(buffer, buffer+length),
      buffer_string_(raw_buffer_.begin(), raw_buffer_.end()) {

}

void ChunkReader_c::AssignBuffer(const uint8_t *buffer, int64_t length) {
  raw_buffer_.assign(buffer, buffer+length);
  buffer_string_.assign(raw_buffer_.begin(), raw_buffer_.end());
}

size_t ChunkReader_c::GetChunkById(const char *id, Chunk_s *chunk) const {
  size_t index = GetBufferIndexOfChunk(id);

  try {
    // check if index is a valid position in the buffer
    if (index == std::string::npos) {
      throw("Could not find chunk ID in buffer.");
    }

    // get size of chunk
    size_t chunk_size = GetChunkDataSize(index);
    if (chunk_size == size_t(-1)) {
      throw("Chunk index out of range.");
    }

    // assign buffer to chunk
    Buffer_t &data = chunk->raw_buffer();
    chunk->AssignBuffer(&raw_buffer_[index], chunk_size + CHUNK_SIZE);
    chunk->InitializeDefaulFields();
    chunk->Initialize();

    return index;

  } catch (const char *err) {
    std::cout << id << ": " << err << std::endl;
    return -1;
  }
}

size_t ChunkReader_c::GetChunkById(const char *idStart,
                                   const char *idEnd,
                                   Chunk_s *chunk) const {
  size_t start = GetBufferIndexOfChunk(idStart);
  size_t end = GetBufferIndexOfChunk(idEnd);

  try {
    // check if start and end index are valid positions in the buffer
    if(start == std::string::npos || end == std::string::npos) {
      throw("Could not find either start or end or both chunk IDs in buffer.");
    }

    // assign buffer to chunk
    chunk->AssignBuffer(&raw_buffer_[start], end - start);
    chunk->InitializeDefaulFields();
    chunk->Initialize();

    return start;

  } catch (const char *err) {
    std::cout << idStart << "/" << idEnd << ": " << err << std::endl;
    return -1;
  }
}

size_t ChunkReader_c::GetChunkByOffset(size_t offset, Chunk_s *chunk) const {
  try {
    // get size of chunk
    size_t chunk_size = GetChunkDataSize(offset);
    if (chunk_size == size_t(-1)) {
      throw("Chunk index out of range.");
    }

    // assign buffer to chunk
    chunk->AssignBuffer(&raw_buffer_[offset], chunk_size + CHUNK_SIZE);
    chunk->InitializeDefaulFields();
    chunk->Initialize();

    return chunk_size;

  } catch (const char *err) {
    std::cout << err << std::endl;
    return -1;
  }
}

Buffer_t& ChunkReader_c::raw_buffer() {
  return raw_buffer_;
}

size_t ChunkReader_c::GetBufferIndexOfChunk(const char *id) const {
  return buffer_string_.find(id, 0);
}

size_t ChunkReader_c::GetChunkDataSize(size_t chunkIndex) const {
  const uint8_t *chunk_size = NULL;

  try {
    chunk_size = &raw_buffer_.at(chunkIndex + CHUNK_SIZE_OFFSET);
  } catch (std::exception &e) {
    std::cout << __LINE__ << " " << __FILE__ << ": " << e.what() << std::endl;
    return -1;
  }

  return *reinterpret_cast<const size_t*>(chunk_size);
}

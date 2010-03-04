#pragma once

#include "../common.h"

typedef std::pair<std::string, uint32_t> FieldOffset_t;
typedef std::map<std::string, uint32_t> FieldOffsets_t;
typedef std::vector<uint8_t> Buffer_t;

static const uint32_t CHUNK_SIZE_OFFSET = 0x4;
static const uint32_t CHUNK_DATA_OFFSET = 0x8;

/*! \brief Base chunk class for all chunked wow data. */
class Chunk_c {
 public:
	Chunk_c(Chunk_c *parent = NULL);
	/* \brief Create a chunk through a buffer. */
	Chunk_c(const uint8_t *buffer, uint32_t length);
  virtual ~Chunk_c();
  /* \brief Retrieves a chunk by its chunk ID.
   * \param id Chunk ID
   * \param length Length of ID
   * \param subChunk Pointer to a Chunk_c instance
   * \return Returns false if given ID can't be found, otherwise true. */
  bool GetSubChunk(const char *id, uint32_t length, Chunk_c *subChunk) const;
  /* \brief Retrieves a chunk by its buffer offset.
   * \param offset Buffer offset
   * \param subChunk Pointer to a Chunk_c instance
   * \return Returns false if offset is out of range, otherwise true. */
	bool GetSubChunk(uint32_t offset, Chunk_c *subChunk) const;

 protected:
	template<typename T> const T& GetField(uint32_t offset) const;
	virtual void Initialize() {}

	Chunk_c *parent_;				//<! Parent chunk

 private:
 	bool AssignBuffer(const uint8_t *buffer, uint32_t length);
 	size_t GetChunkSize(Buffer_t::const_iterator &first) const;

	Buffer_t buffer_;     	//<! Chunk buffer
};

template<typename T>
const T& Chunk_c::GetField(uint32_t offset) const {
	try {
		return *reinterpret_cast<const T*>(&buffer_.at(offset+CHUNK_DATA_OFFSET));
	} catch (std::exception &e) {
		std::cout << __FILE__ << " " << e.what() << std::endl;
	}

	T t;
	return t;
}

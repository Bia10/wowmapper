#pragma once

#include "../common.h"

typedef std::pair<std::string, uint32_t> FieldOffset_t;
typedef std::map<std::string, uint32_t> FieldOffsets_t;
typedef std::vector<uint8_t> Buffer_t;

static const uint32_t CHUNK_SIZE_OFFSET = 0x4;
static const uint32_t CHUNK_DATA_OFFSET = 0x8;

class MpqHandler_c;

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

	const Chunk_c *parent() { return parent_; }

 protected:
	/*! \brief Returns a data field from the buffer, supports PODs only right now.
	 *  \param offset Offset to data field.
	 *  \return Returns a reference to the data. */
	template<typename T> const T& GetField(uint32_t offset) const;
	template<typename T> void CopyDataBlock(Buffer_t &buffer, std::vector<T> *dest);
	template<typename T> void CopyDataBlock(Buffer_t &buffer, uint32_t offset, uint32_t num_elements, std::vector<T> *dest);
	virtual void Initialize() {}

	Chunk_c *parent_;  //<! Parent chunk
	Buffer_t buffer_;  //<! Chunk buffer

 private:
 	bool AssignBuffer(const uint8_t *buffer, uint32_t length);
 	size_t GetChunkSize(Buffer_t::const_iterator &first) const;
};

template<typename T>
const T& Chunk_c::GetField(uint32_t offset) const {
	try {
		return *reinterpret_cast<const T*>(&buffer_.at(offset));
	} catch (std::exception &e) {
		std::cout << __FILE__ << " " << e.what() << std::endl;
	}

	T t;
	return t;
}

template<typename T>
void Chunk_c::CopyDataBlock(Buffer_t &buffer, std::vector<T> *dest) {
  uint8_t *data = reinterpret_cast<uint8_t*>(dest->data());
  std::raw_storage_iterator<uint8_t*, uint8_t> raw_iter(data);

  std::copy(buffer.begin(), buffer.end(), raw_iter);
}

template<typename T>
void Chunk_c::CopyDataBlock(Buffer_t &buffer,
                            uint32_t offset,
                            uint32_t num_elements,
                            std::vector<T> *dest) {
  uint8_t *data = reinterpret_cast<uint8_t*>(dest->data());
  std::raw_storage_iterator<uint8_t*, uint8_t> raw_iter(data);

  std::copy(buffer.begin()+offset,
      buffer.begin()+offset+(num_elements*sizeof(T)), raw_iter);
}

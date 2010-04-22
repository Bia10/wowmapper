#pragma once

#include "../common.h"

class MpqHandler_c;

/*! \brief Base chunk class for all chunked wow data or alike. */
class Chunk_c {
 public:
	/*! \brief Takes the passed buffer and swaps it to make its own.
	 *  \param buf Takes a buffer and swaps it with a clear member buffer. */
	Chunk_c(Buffer_t *buf);
	/*! \brief Usually used to initialize sub chunks so they can reuse the already
	 *         allocated buffer by its parent.
	 *  \param parent Takes a pointer to the parent chunk which provides the buffer.
	 *  \param off Offset to parents sub chunk. */
	Chunk_c(Chunk_c *parent, off_t off);
  virtual ~Chunk_c();

  /* \brief Retrieves a chunk by its chunk ID (unsafe).
   * \param id Chunk ID
   * \param length Length of ID (usually 4)
   * \param subChunk Pointer to a Chunk_c instance
   * \return Returns false if given ID can't be found, otherwise true. */
  //bool GetSubChunk(const std::string &id, Chunk_c *subChunk) const;
  /* \brief Retrieves a chunk by its buffer offset.
   * \param offset Buffer offset
   * \param subChunk Pointer to a Chunk_c instance
   * \return Returns false if offset is out of range, otherwise true. */
	//bool GetSubChunk(off_t off, Chunk_c *subChunk) const;
  off_t GetCurOffset() const;
  size_t GetSize() const;

 protected:

	const Buffer_t& GetBuffer() const;


	/*! \brief Template function to retrieve values in chunks like PODs or simple
	 *         structs made of PODs.
	 *  \param off Offset to data field.
	 *  \return Returns a reference to the data. */
	template<typename T> const T& GetValue(off_t off) const;
	/* \brief Template function to fill any kind of vector with data directly
	 *        from the buffer.
   * \param buf Input buffer (usually chunk buffer).
   * \param off Offset to vectored data.
   * \param num Number of items in the vector to copy.
   * \param dest Destination vector where data gets copied to.
   * \return Return true on success, false otherwise. */
	template<typename T> void CopyVector(const Buffer_t &buf, off_t off, size_t num, std::vector<T> *dest) const;
	template<typename T> void CopyVector(const Buffer_t &buf, std::vector<T> *dest) const;

	Chunk_c *parent_;  //<! Parent chunk
	off_t off_;        //<! Sub chunk offset to parent

	static const off_t SIZE_OFFSET = 0x4; //!< Offset in chunk to size
	static const off_t DATA_OFFSET = 0x8; //!< Offset in chunk to data

 private:
 	size_t GetChunkSize(Buffer_t::const_iterator &first) const;
 	Buffer_t buffer_;  //<! Chunk buffer
};

template<typename T>
const T& Chunk_c::GetValue(off_t off) const {
	off_t real_off = DATA_OFFSET + GetCurOffset() + off;
	return *reinterpret_cast<const T*>(&GetBuffer().at(real_off));
}

template<typename T>
void Chunk_c::CopyVector(const Buffer_t &buf, off_t off,
                         size_t num, std::vector<T> *dest) const {
  uint8_t *data = reinterpret_cast<uint8_t*>(dest->data());
  std::raw_storage_iterator<uint8_t*, uint8_t> raw_iter(data);

  std::copy(buf.begin()+off, buf.begin()+off+(num*sizeof(T)), raw_iter);
}

template<typename T>
void Chunk_c::CopyVector(const Buffer_t &buffer, std::vector<T> *dest) const {
  uint8_t *data = reinterpret_cast<uint8_t*>(dest->data());
  std::raw_storage_iterator<uint8_t*, uint8_t> raw_iter(data);

  std::copy(buffer.begin(), buffer.end(), raw_iter);
}

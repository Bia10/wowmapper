#pragma once

#include "../common.h"

/*! \brief Base chunk class for all chunked wow data or alike. */
class Chunk_c {
 public:
	/*! \brief Takes the passed buffer and swaps it to make its own.
	 *  \param buf Takes a buffer and swaps it with its empty member buffer. */
	Chunk_c(Buffer_t *buf);
	/*! \brief Usually used to initialize sub chunks so they can reuse the already
	 *         allocated buffer by its parent.
	 *  \param parent Takes a pointer to the parent chunk which provides the buffer.
	 *  \param off Offset to parents sub chunk. */
	Chunk_c(Chunk_c *parent, off_t off);
  virtual ~Chunk_c();

  /* \brief Returns current chunk offset by recursively adding up offset.
   * \return Returns current chunk offset. */
  off_t GetCurOffset() const;
  /*! \brief Retrieves the data size of a chunk and returns it.
   *  \return Returns chunk data size. */
  size_t GetSize() const;
  /*! \brief Returns offset to next chunk: current offset + chunk data size + 8.
   *  \return Returns offset to next chunk. */
  off_t GetOffsetToNext() const;

 protected:
  /*! \brief All chunks use only one buffer, so only the root chunk holds the true buffer.
   *  \return Returns the root chunks buffer. */
	const Buffer_t& GetBuffer() const;
	/*! \brief Template function to retrieve values in chunks like PODs or simple
	 *         structs made of PODs.
	 *  \param off Offset to data field.
	 *  \return Returns a reference to the data. */
	template<typename T> const T& GetValue(off_t off, off_t base_off = DATA_OFFSET) const;
	/* \brief Template function to fill any kind of vector with data directly from the buffer.
   * \param buf Input buffer (usually chunk buffer).
   * \param buf_off Offset to vectored data.
   * \param num Number of items in the vector to copy.
   * \param dest_off Offset in destination.
   * \param dest Destination vector where data gets copied to.
   * \return Return true on success, false otherwise.
   * \remark Element type size is determined by sizeof(vector type). */
	template<typename T> void CopyVector(const Buffer_t &buf, off_t buf_off, size_t num, std::vector<T> *dest, off_t dest_off = 0) const;

	Chunk_c *parent_;        //<! Parent chunk
	off_t off_;              //<! Sub chunk offset to parent

	static const off_t SIZE_OFFSET = 0x4; //!< Offset in chunk to size
	static const off_t DATA_OFFSET = 0x8; //!< Offset in chunk to data

 private:
 	size_t GetChunkSize(Buffer_t::const_iterator &first) const;
 	Buffer_t buffer_;  //<! Chunk buffer
};


template<typename T>
const T& Chunk_c::GetValue(off_t off, off_t base_off) const {
	off_t real_off = base_off + GetCurOffset() + off;
	return *reinterpret_cast<const T*>(&GetBuffer().at(real_off));
}

template<typename T>
void Chunk_c::CopyVector(const Buffer_t &buf, off_t buf_off,
                         size_t num, std::vector<T> *dest,
                         off_t dest_off) const {
  uint8_t *data = reinterpret_cast<uint8_t*>(dest->data());
  std::raw_storage_iterator<uint8_t*, uint8_t> raw_iter(data+(dest_off*sizeof(T)));

  std::copy(buf.begin()+buf_off, buf.begin()+buf_off+(num*sizeof(T)), raw_iter);
}

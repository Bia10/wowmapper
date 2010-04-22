#include "chunk_c.h"

Chunk_c::Chunk_c(Buffer_t *buffer)
		: parent_(NULL), off_(0) {
  buffer_.swap(*buffer);
}

Chunk_c::Chunk_c(Chunk_c *parent, off_t off) : parent_(parent), off_(off) {}

Chunk_c::~Chunk_c() {
	parent_ = NULL;
	off_ = 0;
}

off_t Chunk_c::GetCurOffset() const {
  if (parent_ != NULL) {
    return parent_->GetCurOffset() + off_;
  }
  return off_;
}

const Buffer_t& Chunk_c::GetBuffer() const {
  if (parent_ != NULL) {
    return parent_->GetBuffer();
  }
  return buffer_;
}

size_t Chunk_c::GetSize() const {
  off_t real_off = SIZE_OFFSET + GetCurOffset();
  return *reinterpret_cast<const size_t*>(&GetBuffer().at(real_off));
}

/*bool Chunk_c::GetSubChunk(const std::string &id, Chunk_c *subChunk) const {
	Buffer_t::const_iterator found, last;
	found = std::search(buffer_.begin(), buffer_.end(), id.begin(), id.end());

	if (found != buffer_.end()) {
		try {
		  // get first and last iter
			size_t size = GetSize(found);
			found += CHUNK_DATA_OFFSET;
			last = found + size;

			subChunk->buffer_.assign(found, last);
			subChunk->LateInit();
			subChunk->buffer_.clear();
			return true;
		} catch (std::exception &e) {
		  std::cout << __FILE__ << " - GetSubChunk: " << e.what() << std::endl;
		}
	}

	return false;
}

bool Chunk_c::GetSubChunk(uint32_t offset, Chunk_c *subChunk) const {
	try {
		const uint8_t *chunk = &buffer_.at(offset);
		size_t size = *reinterpret_cast<const size_t*>(chunk+CHUNK_SIZE_OFFSET);

		if ((offset + CHUNK_DATA_OFFSET + size) <= buffer_.size()) {
			Buffer_t::const_iterator first = buffer_.begin()+offset+CHUNK_DATA_OFFSET;
			subChunk->buffer_.assign(first, first+size);
			subChunk->LateInit();
			subChunk->buffer_.clear();
			return true;
		}
	} catch (std::exception &e) {
	  std::cout << __FILE__ << " - GetSubChunk: " << e.what() << std::endl;
	}

	return false;
}*/

#include "chunk_c.h"

Chunk_c::Chunk_c(Chunk_c *parent) : parent_(parent) {}

Chunk_c::Chunk_c(const uint8_t *buffer, uint32_t length)
		: parent_(NULL) {
	AssignBuffer(buffer, length);
}

Chunk_c::~Chunk_c() {
	parent_ = NULL;
}

bool Chunk_c::AssignBuffer(const uint8_t *buffer, uint32_t length) {
	try {
		buffer_.assign(buffer, buffer+length);
		return true;
	} catch (std::exception &e) {
		std::cout << __FILE__ << " " << e.what() << std::endl;
	}

	return false;
}

size_t Chunk_c::GetChunkSize(Buffer_t::const_iterator &first) const {
	return *reinterpret_cast<const size_t*>(&(*first) + CHUNK_SIZE_OFFSET);
}

bool Chunk_c::GetSubChunk(const char *id,
													uint32_t length,
													Chunk_c *subChunk) const {
	Buffer_t::const_iterator found, last;
	found = std::search(buffer_.begin(), buffer_.end(), id, id+length);

	if (found != buffer_.end()) {
		try {
		  // get first and last iter
			size_t size = GetChunkSize(found);
			found += CHUNK_DATA_OFFSET;
			last = found + size;

			subChunk->buffer_.assign(found, last);
			subChunk->Initialize();
			subChunk->buffer_.clear();
			return true;
		} catch (std::exception &e) {
			std::cout << __FILE__ << " " << e.what() << std::endl;
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
			subChunk->Initialize();
			subChunk->buffer_.clear();
			return true;
		}
	} catch (std::exception &e) {
		std::cout << __FILE__ << " " << e.what() << std::endl;
	}

	return false;
}

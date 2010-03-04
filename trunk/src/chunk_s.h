#pragma once

#include "chunkreader_c.h"

typedef std::map<std::string, uint32_t> FieldOffsets_t;
typedef std::pair<std::string, uint32_t> FieldOffset_t;

const uint32_t DATA_OFFSET = 0x8;

/*! \brief  Base chunk structure for all chunks. Inherits ChunkReader_c to
 *          allow sub chunk initialization. */
struct Chunk_s : public ChunkReader_c {
  uint32_t id;    //!< Chunk identifier
  uint32_t size;  //!< Data size, total size = data size + 8

 protected:
  FieldOffsets_t field_offsets_;  //!< Field offset define

 public:
  /*! \brief  Chunk_s ctor set the default field offsets for every chunk. Use
   *          the constructor to set them and assign values in Initialize(). */
  Chunk_s() : id(0), size(0) {
    // initialize chunk fields
    field_offsets_.insert(FieldOffset_t("id",   0x0));
    field_offsets_.insert(FieldOffset_t("size", 0x4));
  }

  /*! \brief  Assign default field values to variables */
  void InitializeDefaulFields() {
    AssignValue("id", &id);
    AssignValue("size", &size);
  }

  /*! \brief  Must be defined by every chunk. */
  virtual void Initialize() = 0;

  /*! \brief  Returns a pointer to a data field.
   *  \param  field Field string.
   *  \return Void pointer to data field. */
  void* operator[] (const char *field) {
    // find field offset for key and return its pointer on success
    FieldOffsets_t::const_iterator found = field_offsets_.find(field);
    if(found != field_offsets_.end()) {
      return &raw_buffer_[found->second];
    }

    std::cout << "Field offset: " << field << " not found." << std::endl;
    return NULL;
  }

  /*! \brief  Assigns values of data fields to a given variable.
   *  \param  field Field string.
   *  \param  T Variable to fill with the field data.
   *  \return Returns true if field exists, otherwise false. */
  template<typename T>
  bool AssignValue(const char *field, T *out) {
    // find field offset for key and return its pointer on success
    FieldOffsets_t::const_iterator found = field_offsets_.find(field);
    if(found != field_offsets_.end()) {
      *out = *reinterpret_cast<T*>(&raw_buffer_[found->second]);
      return true;
    }

    std::cout << "Field offset: " << field << " not found." << std::endl;
    return false;
  }

  /*! \brief  Copies ALL data (beginning at +0x8) to a given buffer.
   *  \param  dest Destination buffer. */
  void CopyData(void *dest) const {
    uint8_t *data = reinterpret_cast<uint8_t*>(dest);
    // create a rs iterator to copy the data
    std::raw_storage_iterator<uint8_t*, uint8_t> raw_iter(data);
    std::copy(raw_buffer_.begin() + DATA_OFFSET, raw_buffer_.end(), raw_iter);
  }
};

#pragma once

#include <string>
#include <array>
#include <boost/iostreams/device/mapped_file.hpp>

namespace files
{
  class File
  {
  public:
    using iterator = char*;
    using const_iterator = const char*;

    using raw_data_ptr = void*;
    using const_raw_data_ptr = const void*;

    // Constructor for a file not hashed
    File(const std::string& filepath);

    // Disable copying files
    File(const File& other) = delete;
    File& operator=(const File& other) = delete;

    // Explicitly enable move semantics on a file
    File(File&& other) = default;
    File& operator=(File&& other) = default;

    // Accessors
    const std::string filename_get() const;
    const std::string& filepath_get() const;

    // Get the size from the mapped_file
    size_t size() const;

    // Get a raw pointer on the mapped file data
    raw_data_ptr data();
    const_raw_data_ptr data() const;

    // Iterators to allow <algorithms> usage
    iterator begin();
    const_iterator begin() const;

    iterator end();
    const_iterator end() const;

    // Create an empty file with filename and a size
    static File create_empty_file(const std::string& filename, size_t size);

  private:
    std::string filepath_;
    boost::iostreams::mapped_file file_;

    // Private constructor for an empty file.
    // Should be used only for create_empty_file
    File(const std::string& filepath, size_t size);
  };

  constexpr size_t sha1_hash_size = 20;
  using sha1_hash_elt_type = unsigned char;

  using buffer_type = const void*;

  // Hash a buffer of chars and return the SHA1 hash as a string
  std::string hash_buffer(buffer_type buff, size_t size);

  // Hash a buffer of chars and return the SHA1 hash as a hex
  std::array<sha1_hash_elt_type, sha1_hash_size>
  hash_buffer_hex(buffer_type buff, size_t size);

  // Hash a file. Calling hash_buffer on the mapped area
  std::string hash_file(const File& file);

  // Hash a file. Calling hash_buffer_hex on the mapped area
  std::array<sha1_hash_elt_type, sha1_hash_size>
  hash_file_hex(const File& file);
} // namespace files

#include <mp2p/files/file.hxx>

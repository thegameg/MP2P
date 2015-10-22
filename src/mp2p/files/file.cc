#include <mp2p/files/file.hh>

#include <boost/filesystem.hpp>
#include <openssl/sha.h>

#include <iomanip>
#include <sstream>
#include <cmath>
#include <array>

// Apple deprecated OpenSSL and provides its own
// library called CommonCrypto. Use it to avoid warnings on
// deprecated functions.
#ifdef __APPLE__
#define COMMON_DIGEST_FOR_OPENSSL
#include <CommonCrypto/CommonDigest.h>
#endif

namespace files
{
  namespace b_ios = boost::iostreams;
  namespace b_fs = boost::filesystem;

  namespace // Anonymous namespace
  {
    // Get the number of parts for hashing a file
    size_t
    parts_for_hashing_size(size_t size)
    {
      const size_t mb = 1024 * 1024;
      return size < mb ? 1 : size / mb;
    }

    // Get the size of a part when hashing a file.
    // Be careful for the last part which might be smaller than the others
    size_t
    part_size_for_hashing_size(size_t size, size_t part_id)
    {
      const size_t parts = parts_for_hashing_size(size);
      assert(part_id < parts);

      size_t part_size = std::ceil(static_cast<double>(size) / parts);
      if (part_id == (parts - 1))
      {
        const size_t offset = part_id * part_size;
        if ((offset + part_size) > size)
          part_size -= offset + part_size - size;
      }
      return part_size;
    }

    // Setup tue params of a mapped_file
    auto file_params(const std::string& filepath, size_t size)
    {
      b_ios::mapped_file_params params{filepath};
      params.flags = b_ios::mapped_file_base::readwrite;
      params.new_file_size = size;
      return params;
    }
  }

  File::File(std::string filepath)
    : filepath_{std::move(filepath)}
    , file_{filepath_,
            std::ios_base::binary | std::ios_base::in | std::ios_base::out,
            b_fs::file_size(filepath_)}
  {
  }

  File::File(std::string filepath, size_t size)
    : filepath_{std::move(filepath)}
    , file_{file_params(filepath_, size)}
  {
    // Fix permissions added by boost::mapped_file.
    // 644 should be the right ones.
    using p = b_fs::perms;
    const auto permissions
      = p::owner_read | p::group_read | p::others_read | p::owner_write;

    // Set the permissions
    b_fs::permissions(filepath_, permissions);
  }

  File
  File::create_empty_file(std::string filepath, size_t size)
  {
    return {std::move(filepath), size};
  }

  std::string
  hash_buffer(buffer_type sbuff, size_t size)
  {
    const auto hash = hash_buffer_hex(sbuff, size);

    // Create a string using the hash
    std::ostringstream result;
    for (auto i = 0; i < sha1_hash_size; ++i)
      result << std::hex << std::setw(2) << std::setfill('0')
             << static_cast<int>(hash[i]);

    return result.str();
  }

  // SHA-1 hash a buffer of bytes
  std::array<sha1_hash_elt_type, sha1_hash_size>
  hash_buffer_hex(buffer_type sbuff, size_t size)
  {
    const auto parts = parts_for_hashing_size(size);
    const size_t part_size = std::ceil(static_cast<float>(size) / parts);
    const auto buff = reinterpret_cast<const sha1_hash_elt_type*>(sbuff);
    std::array<sha1_hash_elt_type, sha1_hash_size> hash;

    // Create a SHA_CTX to accumulate the hash
    // SHA1 function needs the whole buffer to hash it
    SHA_CTX context;
    SHA1_Init(&context);

    // FIXME : maybe use it for sending the hash with the part
    for (size_t i = 0; i < parts; ++i)
    {
      // hash a part, update the final one
      const auto hash_size = part_size_for_hashing_size(size, i);
      const auto offset = i * part_size;
      SHA1_Update(&context, buff + offset, hash_size);
    }

    // Get the final hash
    SHA1_Final(hash.data(), &context);

    return hash;
  }

  std::string
  hash_file(const File& file)
  {
    return hash_buffer(file.data(), b_fs::file_size(file.filepath_get()));
  }

  std::array<sha1_hash_elt_type, sha1_hash_size>
  hash_file_hex(const File& file)
  {
    return hash_buffer_hex(file.data(), b_fs::file_size(file.filepath_get()));
  }
}

#pragma once

#include <mp2p/files/file.hh>

#include <boost/filesystem.hpp>

namespace files
{
  inline const std::string&
  File::filepath_get() const
  {
    return filepath_;
  }

  inline const std::string
  File::filename_get() const
  {
    boost::filesystem::path p(filepath_);
    return p.filename().string();
  }

  inline size_t
  File::size() const
  {
    return file_.size();
  }

  inline File::raw_data_ptr
  File::data()
  {
    return file_.data();
  }

  inline File::const_raw_data_ptr
  File::data() const
  {
    return file_.const_data();
  }

  inline File::iterator
  File::begin()
  {
    return file_.begin();
  }

  inline File::const_iterator
  File::begin() const
  {
    return file_.begin();
  }

  inline File::iterator
  File::end()
  {
    return file_.end();
  }

  inline File::const_iterator
  File::end() const
  {
    return file_.end();
  }
} // namespace files

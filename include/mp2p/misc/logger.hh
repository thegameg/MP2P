#pragma once

#include <iostream>

namespace misc
{
  class Logger
  {
  public:
    static auto&
    cout()
    {
      return std::cout;
    }

    static auto&
    cerr()
    {
      return std::cerr;
    }
  };
} // namespace misc

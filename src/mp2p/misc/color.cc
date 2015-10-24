#include <mp2p/misc/color.hh>

#include <string>
#include <iostream>

namespace misc
{
  namespace color
  {
    static const std::string white_color_escape = "\033[0m";
    static const std::string red_color_escape = "\033[91m";
    static const std::string blue_color_escape = "\033[94m";
    static const std::string cyan_color_escape = "\033[96m";
    static const std::string green_color_escape = "\033[92m";
    static const std::string yellow_color_escape = "\033[93m";
    static const std::string purple_color_escape = "\033[95m";

    std::ostream&
    w(std::ostream& o)
    {
      return o << white_color_escape;
    }

    std::ostream&
    r(std::ostream& o)
    {
      return o << red_color_escape;
    }

    std::ostream&
    b(std::ostream& o)
    {
      return o << blue_color_escape;
    }

    std::ostream&
    c(std::ostream& o)
    {
      return o << cyan_color_escape;
    }

    std::ostream&
    g(std::ostream& o)
    {
      return o << green_color_escape;
    }

    std::ostream&
    y(std::ostream& o)
    {
      return o << yellow_color_escape;
    }

    std::ostream&
    p(std::ostream& o)
    {
      return o << purple_color_escape;
    }
  } // namespace color
} // namespace misc

#pragma once

#include <iostream> // std::cout

namespace misc
{
  namespace color
  {
    // White
    std::ostream& w(std::ostream& o = std::cout);
    // Red
    std::ostream& r(std::ostream& o = std::cout);
    // Blue
    std::ostream& b(std::ostream& o = std::cout);
    // Cyan
    std::ostream& c(std::ostream& o = std::cout);
    // Green
    std::ostream& g(std::ostream& o = std::cout);
    // Yellow
    std::ostream& y(std::ostream& o = std::cout);
    // Purple
    std::ostream& p(std::ostream& o = std::cout);
  } // namespace color
} // namespace misc

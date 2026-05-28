// HTTP/Common.hpp
#pragma once

#include <string>

namespace http
{

  enum class version
  {
    Http1_0,
    Http1_1,
    Unsupported
  };

  struct header
  {
    std::string Name;
    std::string Value;
  };

} // namespace http

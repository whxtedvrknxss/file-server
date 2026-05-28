// HTTP/RequestBuilder.hpp
#pragma once

#include <span>
#include <string>
#include <vector>

#include "Common.hpp"

namespace http
{

  enum class method
  {
    Get,
    Head,
    Post,
    Put,
    Delete,
    Connect,
    Error,
  };

  struct request
  {
    version Version;
    std::vector<header> Headers;
    std::vector<char> Body;

    method Method;
    std::string URI;
  };

  class request_builder
  {
  public:
    request_builder() = default;

    void OnMethod(std::string_view Method);
    void OnURI(std::string_view URI);
    void OnHeader(std::string_view Name, std::string_view Value);
    void OnVersion(uint8_t Major, uint8_t Minor);
    void OnBody(std::span<const char> Chunk);

    void OnComplete();

    [[nodiscard]] request Release();

    void Reset();

  private:
    request Request;
  };

} // namespace http

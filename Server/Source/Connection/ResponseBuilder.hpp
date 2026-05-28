// HTTP/BuilderResponse.hpp
#pragma once

#include <vector>

#include "Common.hpp"

namespace http
{

  enum class status_code : uint16_t
  {
    Ok = 200,
    Created = 201,
    BadRequest = 400,
    NotFound = 404,
    ContentTooLarge = 413,
    URITooLong = 414,
    IAmATeapot = 418,
    InternalServerError = 500,
    NotImplemented = 501,
    HTTPVersionNotSupported = 505,
  };

  struct response
  {
    version Version;
    std::vector<header> Headers;
    std::vector<char> Body;

    status_code Status;
  };

  class response_builder
  {
  public:
    response_builder() = default;

    void Build(status_code Status);

    void Reset();

  private:
    response_builder &AddStatusCode(status_code Status);
    response_builder &AddVersion(version Version);
    response_builder &AddHeader(header Header);

  private:
    static std::string StatusToString(status_code Code);
    static std::string VersionToString(version Version);

  private:
    response Response;
    std::vector<char> ResponseBytes;
  };

} // namespace http

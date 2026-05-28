#include "ResponseBuilder.hpp"

#include <format>

namespace http
{
  void response_builder::Build(status_code Status)
  {
    ResponseBytes.append_range(std::format("{} {}\r\n", VersionToString(Response.Version),
                                           StatusToString(Response.Status)));
    for (const auto &[Name, Value] : Response.Headers)
    {
      ResponseBytes.append_range(std::format("{}: {}\r\n", Name, Value));
    }

    // TODO: change response body formatting(bodies can get very large)
    ResponseBytes.append_range(std::format("\r\n{}\r\n\r\n", Response.Body));
  }

  void response_builder::Reset()
  {
    Response = {};
  }

  response_builder &response_builder::AddStatusCode(status_code Status)
  {
    Response.Status = Status;
    return *this;
  }

  response_builder &response_builder ::AddVersion(version Version)
  {
    Response.Version = Version;
    return *this;
  }

  response_builder &response_builder::AddHeader(header Header)
  {
    Response.Headers.push_back(std::move(Header));
    return *this;
  }

  std::string response_builder::StatusToString(status_code Code)
  {
    switch (Code)
    {
      case status_code::Ok:
      {
        return "200 Ok";
      }
      case status_code::Created:
      {
        return "201 Created";
      }
      case status_code::BadRequest:
      {
        return "400 Bad Request";
      }
      case status_code::NotFound:
      {
        return "404 Not Found";
      }
      case status_code::ContentTooLarge:
      {
        return "413 Content Too Large";
      }
      case status_code::URITooLong:
      {
        return "414 URI Too Long";
      }
      case status_code::IAmATeapot:
      {
        return "418 I'm a teapot";
      }

      default:
      case status_code::InternalServerError:
      {
        return "500 Internal Server Error";
      }

      case status_code::NotImplemented:
      {
        return "501 Not Implemented";
      }
      case status_code::HTTPVersionNotSupported:
      {
        return "505 HTTP Version Not Supported";
      }
    }
  }

  std::string response_builder::VersionToString(version Version)
  {
    switch (Version)
    {
      case version::Http1_0:
      {
        return "HTTP/1.0";
      }
      default:
      case version::Http1_1:
      {
        return "HTTP/1.1";
      }
    }
  }

} // namespace http

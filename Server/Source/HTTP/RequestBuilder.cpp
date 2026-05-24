#include "RequestBuilder.hpp"

namespace http
{

  void request_builder::OnMethod(std::string_view Method)
  {
    if (Method == "GET")
    {
      Request.Method = method::Get;
    }
    else if (Method == "HEAD")
    {
      Request.Method = method::Head;
    }
    else if (Method == "POST")
    {
      Request.Method = method::Post;
    }
    else if (Method == "PUT")
    {
      Request.Method = method::Put;
    }
    else if (Method == "DELETE")
    {
      Request.Method = method::Delete;
    }
    else if (Method == "CONNECT")
    {
      Request.Method = method::Connect;
    }
  }

  void request_builder::OnURI(std::string_view URI)
  {
    Request.URI.append(URI);
  }

  void request_builder::OnVersion(uint8_t Major, uint8_t Minor)
  {
    if (Major == 1 && (Minor == 1 || Minor == 0))
    {
      Request.Version = (Minor == 0) ? version::Http1_0 : version::Http1_1;
    }
    else
    {
      Request.Version = version::Unsupported;
    }
  }

  void request_builder::OnHeader(std::string_view Name, std::string_view Value)
  {
    Request.Headers.push_back(header{std::string(Name), std::string(Value)});
  }

  void request_builder::OnBody(std::span<const char> Chunk)
  {
    Request.Body.append_range(Chunk);
  }

} // namespace http

#include "Parser.hpp"

namespace http
{

  parse_result request_parser::Parse(std::span<const std::byte> Content)
  {
    return parse_result();
  }

  message request_parser::ReleaseMessage()
  {
    return std::exchange(Message, {});
  }

  void request_parser::ParseMethod()
  {
    // if (Method == "GET")
    // {
    //   Message.StartLine.Method = http_method::Get;
    //   return;
    // }
    // if (Method == "HEAD")
    // {
    //   Message.StartLine.Method = http_method::Head;
    //   return;
    // }
    // if (Method == "POST")
    // {
    //   Message.StartLine.Method = http_method::Post;
    //   return;
    // }
    // if (Method == "PUT")
    // {
    //   Message.StartLine.Method = http_method::Put;
    //   return;
    // }
    // if (Method == "CONNECT")
    // {
    //   Message.StartLine.Method = http_method::Connect;
    //   return;
    // }
    // if (Method == "OPTIONS")
    // {
    //   Message.StartLine.Method = http_method::Options;
    //   return;
    // }
    // if (Method == "TRACE")
    // {
    //   Message.StartLine.Method = http_method::Trace;
    //   return;
    // }
    // if (Method == "PATCH")
    // {
    //   Message.StartLine.Method = http_method::Patch;
    //   return;
    // }
    throw parsing_error("Unknown method");
  }

  void request_parser::ParseURI()
  {
    // Message.StartLine.URI.assign(URI);
  }

  void request_parser::ParseVersion()
  {
    // if (Version == "HTTP/1.1")
    // {
    //   Message.StartLine.Version = http_version::Http1_0;
    //   return;
    // }
    // if (Version == "HTTP/2")
    // {
    //   Message.StartLine.Version = http_version::Http2_0;
    //   return;
    // }

    throw parsing_error("Unknown version");
  }

  void request_parser::ParseHeader()
  {
  }

  void request_parser::ParseBody()
  {
  }

} // namespace http

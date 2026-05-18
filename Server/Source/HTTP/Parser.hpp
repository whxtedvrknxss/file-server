// HttpParser.hpp
#pragma once

#include <span>
#include <string>
#include <stdexcept>
#include <vector>

namespace http
{

  struct parsing_error : public std::invalid_argument
  {
  public:
    parsing_error(const std::string &Message) : std::invalid_argument(Message.c_str())
    {
    }

    parsing_error(const char *Message) : std::invalid_argument(Message)
    {
    }
  };

  enum class parse_status
  {
    Complete,
    Incomplete,
    Error
  };

  enum class parse_error
  {
    None,
    InvalidMethod,
    InvalidURI,
    InvalidVersion,
    MalformedHeader,
    InvalidContentLength,
    UnexpectedEof,
    BodyTooLarge,
  };

  enum class method
  {
    Get,
    Head,
    Post,
    Put,
    Delete,
    Connect,
    Options,
    Trace,
    Patch,
  };

  enum class version
  {
    Http1_1,
    Http2_0,
  };

  struct parse_result
  {
    parse_status Status;
    parse_error Error;
    std::size_t BytesConsumed;
  };

  struct header
  {
    std::string Name;
    std::string Value;
  };

  struct message
  {
    struct
    {
      method Method;
      std::string URI;
      version Version;
    } StartLine;

    std::vector<header> Headers;
    std::string Body;
  };

  class request_parser
  {
  public:
    request_parser() = default;

    parse_result Parse(std::span<const std::byte> Data);

    [[nodiscard]] bool HasMessage() const noexcept;

    [[nodiscard]] message ReleaseMessage();

    void Reset() noexcept;

  private:
    enum class state
    {
      Method,
      URI,
      Version,
      HeaderName,
      HeaderValue,
      HeadersDone,
      Body,
      Complete,
      Error
    };

  private:
    void ParseMethod();
    void ParseURI();
    void ParseVersion();
    void ParseHeader();
    void ParseBody();

  private:
    message Message;

    state State = state::Method;
    parse_error Error = parse_error::None;

    std::size_t ContentLength{};
    std::size_t ParsedBodyBytes{};
  };

} // namespace http

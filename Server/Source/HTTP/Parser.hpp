// HttpParser.hpp
#pragma once

#include <span>
#include <string>
#include <stdexcept>
#include <vector>

#include <llhttp.h>

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

  struct parse_result
  {
    parse_status Status;
    parse_error Error;
  };

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

  enum class version
  {
    Http1_0,
    Http1_1,
  };

  struct header
  {
    std::string Name;
    std::string Value;
  };

  struct message
  {
    method Method;
    std::string URI;
    version Version;
    std::vector<header> Headers;
    std::string Body;
  };

  class request_parser
  {
  public:
    request_parser() noexcept;

    [[nodiscard]] parse_result Parse(std::span<const char> Data);

    [[nodiscard]] bool HasMessage() const noexcept
    {
      return MessageReady;
    }

    [[nodiscard]] message ReleaseMessage()
    {
      return std::exchange(Message, {});
    }

    void Reset() noexcept;

  private:
    parse_error TranslateError(llhttp_errno_t Err);

  private:
    // callbacks for llhttp
    static int OnMethod(llhttp_t *Parser, const char *At, size_t Length);
    static int OnURI(llhttp_t *Parser, const char *At, size_t Length);
    static int OnHeaderName(llhttp_t *Parser, const char *At, size_t Length);
    static int OnHeaderValue(llhttp_t *Parser, const char *At, size_t Length);
    static int OnHeadersComplete(llhttp_t *Parser);
    static int OnBody(llhttp_t *Parser, const char *At, size_t Length);
    static int OnMessageComplete(llhttp_t *Parser);

  private:
    message Message;

    bool MessageReady;
    bool ParsingHeaderValue;
    header CurrentHeader;

    std::size_t ContentLength{};
    std::size_t ParsedBodyBytes{};

    llhttp_t Parser;
    llhttp_settings_t Settings;
  };

} // namespace http

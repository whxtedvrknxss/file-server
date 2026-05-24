// HttpParser.hpp
#pragma once

#include <span>

#include <llhttp.h>

#include "RequestBuilder.hpp"

namespace http
{

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

  class request_parser
  {
  public:
    request_parser(request_builder &Builder) noexcept;

    [[nodiscard]] parse_result Parse(std::span<const char> Data);

    void Reset() noexcept;

  private:
    parse_error TranslateError(llhttp_errno_t Err);

  private:
    static int OnMethod(llhttp_t *, const char *, size_t);
    static int OnURI(llhttp_t *, const char *, size_t);
    static int OnHeaderName(llhttp_t *, const char *, size_t);
    static int OnHeaderValue(llhttp_t *, const char *, size_t);
    static int OnHeadersComplete(llhttp_t *);
    static int OnBody(llhttp_t *, const char *, size_t);
    static int OnMessageComplete(llhttp_t *);

  private:
    bool Complete;
    bool ParsingHeaderValue;
    header CurrentHeader;

    llhttp_t Parser;
    llhttp_settings_t Settings;

    request_builder *Builder;
  };

} // namespace http

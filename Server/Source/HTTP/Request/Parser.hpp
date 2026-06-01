#pragma once

#include <llhttp.h>

#include <span>

#include "Builder.hpp"

namespace http {

enum class ParseStatus : std::uint8_t { Complete, Incomplete, Error };

enum class ParseError : std::uint8_t {
  None,
  InvalidMethod,
  InvalidURI,
  InvalidVersion,
  MalformedHeader,
  InvalidContentLength,
  UnexpectedEof,
  BodyTooLarge,
};

struct ParseResult {
  ParseStatus status;
  ParseError error;
};

class RequestParser {
 public:
  RequestParser(RequestBuilder &builder) noexcept;

  [[nodiscard]] ParseResult Parse(std::span<const char> data);

  void Reset() noexcept;

 private:
  static ParseError TranslateError(llhttp_errno_t Err);

 private:
  static int OnMethod(llhttp_t *, const char *, size_t);
  static int OnURI(llhttp_t *, const char *, size_t);
  static int OnHeaderName(llhttp_t *, const char *, size_t);
  static int OnHeaderValue(llhttp_t *, const char *, size_t);
  static int OnHeadersComplete(llhttp_t *);
  static int OnBody(llhttp_t *, const char *, size_t);
  static int OnMessageComplete(llhttp_t *);

 private:
  bool complete_;
  bool parsing_header_value_;
  Header current_header_;

  llhttp_t parser_;
  llhttp_settings_t settings_;

  RequestBuilder *builder_;
};

}  // namespace http

#include "Parser.hpp"

#include <llhttp.h>

namespace http
{
  request_parser::request_parser() noexcept
      : ParsingHeaderValue{false}, ContentLength{}, ParsedBodyBytes{}, MessageReady{false}
  {
    llhttp_settings_init(&Settings);

    Settings.on_method = OnMethod;
    Settings.on_url = OnURI;
    Settings.on_header_field = OnHeaderName;
    Settings.on_header_value = OnHeaderValue;
    Settings.on_headers_complete = OnHeadersComplete;
    Settings.on_body = OnBody;
    Settings.on_message_complete = OnMessageComplete;

    llhttp_init(&Parser, llhttp_type_t::HTTP_REQUEST, &Settings);

    Parser.data = this;
  }

  parse_result request_parser::Parse(std::span<const char> Content)
  {
    parse_result Result;

    llhttp_errno_t Err = llhttp_execute(&Parser, Content.data(), Content.size());

    Result.Status = (Err == HPE_OK) ? parse_status::Complete : parse_status::Error;
    Result.Error = TranslateError(Err);

    return Result;
  }

  void request_parser::Reset() noexcept
  {
    llhttp_reset(&Parser);

    Message = {};
  }

  parse_error request_parser::TranslateError(llhttp_errno_t Err)
  {
    switch (Err)
    {
      case llhttp_errno_t::HPE_OK:
      {
        return parse_error::None;
      }
      case llhttp_errno_t::HPE_INVALID_METHOD:
      {
        return parse_error::InvalidMethod;
      }
      case llhttp_errno_t::HPE_INVALID_VERSION:
      {
        return parse_error::InvalidVersion;
      }
      case llhttp_errno_t::HPE_INVALID_HEADER_TOKEN:
      {
        return parse_error::MalformedHeader;
      }
      default:
      {
        return parse_error::UnexpectedEof;
      }
    }
  }

  int request_parser::OnMethod(llhttp_t *Parser, const char *At, size_t Length)
  {
    auto *Self = static_cast<request_parser *>(Parser->data);

    method &Method = Self->Message.Method;

    switch (Parser->method)
    {
      case llhttp_method_t::HTTP_GET:
      {
        Method = method::Get;
        break;
      }
      case llhttp_method::HTTP_HEAD:
      {
        Method = method::Head;
        break;
      }
      case llhttp_method_t::HTTP_POST:
      {
        Method = method::Post;
        break;
      }
      case llhttp_method_t::HTTP_PUT:
      {
        Method = method::Put;
        break;
      }
      case llhttp_method_t::HTTP_DELETE:
      {
        Method = method::Delete;
        break;
      }
      case llhttp_method_t::HTTP_CONNECT:
      {
        Method = method::Connect;
        break;
      }
      default:
      {
        return llhttp_errno_t::HPE_INVALID_METHOD;
      }
    }

    return llhttp_errno_t::HPE_OK;
  }

  int request_parser::OnURI(llhttp_t *Parser, const char *At, size_t Length)
  {
    auto *Self = static_cast<request_parser *>(Parser->data);

    Self->Message.URI.append(At, Length);

    return llhttp_errno_t::HPE_OK;
  }

  int request_parser::OnHeaderName(llhttp_t *Parser, const char *At, size_t Length)
  {
    auto *Self = static_cast<request_parser *>(Parser->data);

    header &CurrentHeader = Self->CurrentHeader;
    if (Self->ParsingHeaderValue)
    {
      Self->Message.Headers.push_back(std::move(CurrentHeader));
      Self->CurrentHeader = {};
      Self->ParsingHeaderValue = false;
    }

    CurrentHeader.Name.append(At, Length);

    return llhttp_errno_t::HPE_OK;
  }

  int request_parser::OnHeaderValue(llhttp_t *Parser, const char *At, size_t Length)
  {
    auto *Self = static_cast<request_parser *>(Parser->data);
    header &CurrentHeader = Self->CurrentHeader;

    Self->ParsingHeaderValue = true;
    CurrentHeader.Value.append(At, Length);

    return llhttp_errno_t::HPE_OK;
  }

  int request_parser::OnHeadersComplete(llhttp_t *Parser)
  {
    auto *Self = static_cast<request_parser *>(Parser->data);

    header &CurrentHeader = Self->CurrentHeader;
    if (!Self->CurrentHeader.Name.empty())
    {
      Self->Message.Headers.push_back(std::move(CurrentHeader));
    }

    version &Version = Self->Message.Version;
    if (Parser->http_major == 1)
    {
      if (Parser->http_minor == 0)
      {
        Version = version::Http1_0;
      }
      else if (Parser->http_minor == 1)
      {
        Version = version::Http1_1;
      }
      else
      {
        return llhttp_errno_t::HPE_INVALID_VERSION;
      }
    }
    else if (Parser->http_major == 2)
    {
      return llhttp_errno_t::HPE_INVALID_VERSION;
    }

    return llhttp_errno_t::HPE_OK;
  }

  int request_parser::OnBody(llhttp_t *Parser, const char *At, size_t Length)
  {
    auto *Self = static_cast<request_parser *>(Parser->data);
    std::string &Body = Self->Message.Body;

    Body.append(At, Length);

    return llhttp_errno_t::HPE_OK;
  }

  int request_parser::OnMessageComplete(llhttp_t *Parser)
  {
    auto *Self = static_cast<request_parser *>(Parser->data);

    return llhttp_errno_t::HPE_OK;
  }

} // namespace http

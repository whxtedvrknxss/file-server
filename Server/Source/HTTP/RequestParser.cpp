#include "RequestParser.hpp"

#include <llhttp.h>

namespace http
{
  request_parser::request_parser(request_builder &Builder) noexcept : Builder{&Builder}, Complete{false}
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
    Self->Builder->OnMethod(std::string_view(At, Length));

    return llhttp_errno_t::HPE_OK;
  }

  int request_parser::OnURI(llhttp_t *Parser, const char *At, size_t Length)
  {
    auto *Self = static_cast<request_parser *>(Parser->data);

    Self->Builder->OnURI(std::string_view(At, Length));
    return llhttp_errno_t::HPE_OK;
  }

  int request_parser::OnHeaderName(llhttp_t *Parser, const char *At, size_t Length)
  {
    auto *Self = static_cast<request_parser *>(Parser->data);

    if (Self->ParsingHeaderValue)
    {
      Self->Builder->OnHeader(Self->CurrentHeader.Name, Self->CurrentHeader.Value);
      Self->CurrentHeader = {};
    }
    Self->CurrentHeader.Name.append(At, Length);
    return llhttp_errno_t::HPE_OK;
  }

  int request_parser::OnHeaderValue(llhttp_t *Parser, const char *At, size_t Length)
  {
    auto *Self = static_cast<request_parser *>(Parser->data);

    Self->ParsingHeaderValue = true;
    Self->CurrentHeader.Value.append(At, Length);
    return llhttp_errno_t::HPE_OK;
  }

  int request_parser::OnHeadersComplete(llhttp_t *Parser)
  {
    auto *Self = static_cast<request_parser *>(Parser->data);

    if (!Self->CurrentHeader.Name.empty() && !Self->CurrentHeader.Value.empty())
    {
      Self->Builder->OnHeader(Self->CurrentHeader.Name, Self->CurrentHeader.Value);
      Self->CurrentHeader = {};
    }

    Self->Builder->OnVersion(Parser->http_major, Parser->http_minor);

    return llhttp_errno_t::HPE_OK;
  }

  int request_parser::OnBody(llhttp_t *Parser, const char *At, size_t Length)
  {
    auto *Self = static_cast<request_parser *>(Parser->data);
    Self->Builder->OnBody(std::string_view(At, Length));
    return llhttp_errno_t::HPE_OK;
  }

  int request_parser::OnMessageComplete(llhttp_t *Parser)
  {
    auto *Self = static_cast<request_parser *>(Parser->data);

    return llhttp_errno_t::HPE_OK;
  }

} // namespace http

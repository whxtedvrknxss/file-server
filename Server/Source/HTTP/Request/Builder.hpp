#pragma once

#include <span>
#include <string>
#include <vector>

#include "HTTP/Common.hpp"

namespace http {

enum class Method : std::uint8_t {
  Get,
  Head,
  Post,
  Put,
  Delete,
  Invalid,
};

struct Request {
  Version version;
  std::vector<Header> headers;
  std::vector<char> body;

  Method method;
  std::string uri;
};

class RequestBuilder {
 public:
  using BodySink = std::function<void(std::span<const char>)>;

  RequestBuilder() = default;

  // TODO: finish file sinking 
  // void SetBodySink(BodySink sink) { body_data_sink_ = sink; }

  void OnMethod(std::string_view method);
  void OnURI(std::string_view uri);
  void OnHeader(std::string_view name, std::string_view value);
  void OnVersion(uint8_t major, uint8_t minor);
  void OnBody(std::span<const char> chunk);

  void OnComplete();

  [[nodiscard]] Request Release();

  void Reset();

 private:
  Request request_;
  std::string method_buffer_;
  BodySink body_data_sink_;
};

}  // namespace http

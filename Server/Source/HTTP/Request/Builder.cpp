#include "Builder.hpp"

namespace http {

void RequestBuilder::OnMethod(std::string_view method) {
  method_buffer_.append(method);
}

void RequestBuilder::OnURI(std::string_view uri) { request_.uri.append(uri); }

void RequestBuilder::OnVersion(uint8_t major, uint8_t minor) {
  if (major == 1 && (minor == 1 || minor == 0)) {
    request_.version = (minor == 0) ? Version::Http1_0 : Version::Http1_1;
  } else {
    request_.version = Version::Unsupported;
  }
}

void RequestBuilder::OnHeader(std::string_view name, std::string_view value) {
  request_.headers.push_back(
      Header{.name = std::string(name), .value = std::string(value)});
}

void RequestBuilder::OnBody(std::span<const char> chunk) {
  request_.body.append_range(chunk);
}

}  // namespace http

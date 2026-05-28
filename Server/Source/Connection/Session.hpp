// Session.hpp
#pragma once

#include <memory>

#include <asio.hpp>

#include "RequestBuilder.hpp"
#include "RequestParser.hpp"
#include "ResponseBuilder.hpp"

class router;

class session : public std::enable_shared_from_this<session>
{
  using tcp = asio::ip::tcp;

public:
  explicit session(tcp::socket Socket, router &Router)
      : Socket{std::move(Socket)}, RequestParser{RequestBuilder}, Router{&Router}
  {
  }

  void Start();

private:
  void Read();
  void Write(size_t BytesAmount);

  void OnRead(size_t BytesAmount);

private:
  static constexpr std::size_t MaxLength = 4096;

  tcp::socket Socket;
  std::array<char, MaxLength> ReadBuffer;
  std::array<char, MaxLength> WriteBuffer;

  http::request_builder RequestBuilder;
  http::request_parser RequestParser;

  http::response_builder ResponseBuilder;
  router *Router;
};

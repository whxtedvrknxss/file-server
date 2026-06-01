#pragma once

#include <asio.hpp>
#include <memory>

#include "HTTP/Request/Builder.hpp"
#include "HTTP/Request/Parser.hpp"
#include "HTTP/ResponseBuilder.hpp"

class Router;

class Session : public std::enable_shared_from_this<Session> {
  using tcp = asio::ip::tcp;

 public:
  explicit Session(tcp::socket Socket, Router &Router)
      : Socket{std::move(Socket)},
        RequestParser{RequestBuilder},
        Router{&Router} {}

  void Start();

 private:
  void Read();
  void Write(std::size_t bytes_amount);

  void OnRead(std::size_t bytes_amount);

 private:
  static constexpr std::size_t kMaxLength = 4096;

  tcp::socket Socket;
  std::array<char, kMaxLength> ReadBuffer;
  std::array<char, kMaxLength> WriteBuffer;

  http::RequestBuilder RequestBuilder;
  http::RequestParser RequestParser;

  http::ResponseBuilder ResponseBuilder;
  Router *Router;
};

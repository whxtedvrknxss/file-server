// Session.hpp
#pragma once

#include <memory>

#include <asio.hpp>

#include "HTTP/RequestParser.hpp"

class session : public std::enable_shared_from_this<session>
{
  using tcp = asio::ip::tcp;

public:
  explicit session(tcp::socket Socket) : Socket{std::move(Socket)}, Builder{}, Parser{Builder}
  {
  }

  void Start();

private:
  void Read();
  void Write(size_t Length);

private:
  tcp::socket Socket;
  std::array<char, 4096> ReadBuffer;
  std::array<char, 4096> WriteBuffer;

  http::request_builder Builder;
  http::request_parser Parser;
};

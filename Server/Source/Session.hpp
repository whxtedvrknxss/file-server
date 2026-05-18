// Session.hpp
#pragma once

#include <memory>

#include <asio.hpp>

class session : public std::enable_shared_from_this<session>
{
  using tcp = asio::ip::tcp;

public:
  explicit session(tcp::socket Socket) : Socket{std::move(Socket)}
  {
  }

  void Start();

private:
  void DoRead();
  void DoWrite(size_t Length);

private:
  tcp::socket Socket;
  std::array<char, 4096> Data;
};

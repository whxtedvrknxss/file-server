// HttpServer.h
#pragma once

#include <asio.hpp>

class server
{
  using tcp = asio::ip::tcp;

public:
  server() : Acceptor(IoContext, tcp::endpoint(tcp::v4(), 8080))
  {
  }

  void Run();
  void Listen();

private:
  asio::io_context IoContext;
  tcp::acceptor Acceptor;
};

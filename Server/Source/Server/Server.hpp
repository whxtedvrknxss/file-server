// HttpServer.h
#pragma once

#include <asio.hpp>

#include "HTTP/Parser.hpp"

class server
{
  using tcp = asio::ip::tcp;

public:
  server();

  void Run();
  void Listen();

private:
  asio::io_context IoContext;
  tcp::acceptor Acceptor;
  http::request_parser Parser;
};

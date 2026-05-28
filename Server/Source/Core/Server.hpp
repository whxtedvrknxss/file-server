// HttpServer.h
#pragma once

#include <asio.hpp>

#include "Connection/SessionManager.hpp"

#include "Work/ThreadPool.hpp"

class router
{
};

class server
{
  using tcp = asio::ip::tcp;

public:
  server(uint16_t Port);

  void Run();
  void Listen();
  void DoAccept();

private:
  asio::io_context IoContext;
  tcp::acceptor Acceptor;

  session_manager SessionManager;

  router Router;

  thread_pool ThreadPool;
};

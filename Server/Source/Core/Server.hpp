// HttpServer.h
#pragma once

#include <asio.hpp>

#include "Connection/SessionManager.hpp"
#include "Router.hpp"
#include "Work/ThreadPool.hpp"

class Server {
  using tcp = asio::ip::tcp;

 public:
  Server(uint16_t Port);

  void Run();
  void Listen();
  void DoAccept();

 private:
  asio::io_context io_context_;
  tcp::acceptor acceptor_;

  SessionManager session_manager_;

  Router router_;

  ThreadPool thread_pool_;
};

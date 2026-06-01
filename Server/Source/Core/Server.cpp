#include "Server.hpp"

#include "Utility/Logging.hpp"

Server::Server(uint16_t port)
    : acceptor_(io_context_, tcp::endpoint(tcp::v4(), port)) {
  LOG_INFO("The Server started listening on port {}", port);
  DoAccept();
}

void Server::Run() {
  for (size_t i{}; i < thread_pool_.Size(); ++i) {
  }
}

void Server::Listen() {
  DoAccept();
  io_context_.run();
}

void Server::DoAccept() {
  auto func = [this](std::error_code ErrorCode, tcp::socket Socket) {
    if (!ErrorCode) {
      auto session = std::make_shared<Session>(std::move(Socket), router_);

      session_manager_.Start(session);
    }

    Listen();
  };
  acceptor_.async_accept(asio::make_strand(acceptor_.get_executor()), func);
}

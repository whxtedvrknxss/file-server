#include "Server.hpp"

server::server(uint16_t Port) : Acceptor(IoContext, tcp::endpoint(tcp::v4(), Port))
{
  DoAccept();
}

void server::Run()
{
  for (size_t I{}; I < ThreadPool.Size(); ++I)
  {
    ThreadPool.Enqueue([this] { IoContext.run(); });
  }
}

void server::Listen()
{
  DoAccept();
  IoContext.run();
}

void server::DoAccept()
{
  auto Fn = [this](std::error_code ErrorCode, tcp::socket Socket) {
    if (!ErrorCode)
    {
      auto Session = std::make_shared<session>(std::move(Socket), Router);

      SessionManager.Start(Session);
    }

    Listen();
  };
  Acceptor.async_accept(asio::make_strand(Acceptor.get_executor()), Fn);
}

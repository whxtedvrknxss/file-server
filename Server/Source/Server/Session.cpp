#include "Session.hpp"

#include <print>

void session::Start()
{
  DoRead();
}

void session::DoRead()
{
  auto Self = shared_from_this();

  Socket.async_read_some(asio::buffer(Data),
                         [this, Self](std::error_code ErrorCode, size_t Length)
                         {
                           if (!ErrorCode)
                           {
                             std::println("Client: {}", std::string(Data.data(), Length));
                           }
                           DoWrite(Length);
                         });
}

void session::DoWrite(size_t Length)
{
  auto Self = shared_from_this();
  asio::async_write(Socket, asio::buffer(Data, Length),
                    [this, Self](asio::error_code ErrorCode, size_t)
                    {
                      if (!ErrorCode)
                      {
                        DoRead();
                      }
                    });
}
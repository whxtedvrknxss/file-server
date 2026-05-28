#include "Session.hpp"

#include <print>

void session::Start()
{
  Read();
}

void session::Read()
{
  auto Self{shared_from_this()};
  auto Fn = [Self](std::error_code ErrorCode, size_t Length)
  {
    Self->OnRead(Length);
  };
  auto BindExecutor = asio::bind_executor(Socket.get_executor(), Fn);
  Socket.async_read_some(asio::buffer(ReadBuffer), BindExecutor);
}

void session::Write(size_t Length)
{
  auto Self{shared_from_this()};
  auto Fn = [this, Self](asio::error_code ErrorCode, size_t)
  {
    if (!ErrorCode)
    {
      Read();
    }
  };
  auto BindExecutor = asio::bind_executor(Socket.get_executor(), Fn);
  asio::async_write(Socket, asio::buffer(WriteBuffer, Length), BindExecutor);
}

void session::OnRead(size_t BytesAmount)
{
  auto Result = RequestParser.Parse({ReadBuffer.data(), BytesAmount});
}

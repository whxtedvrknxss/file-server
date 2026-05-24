#include "Session.hpp"

#include <print>
#include <string>

void session::Start()
{
  Read();
}

void session::Read()
{
  auto Self = shared_from_this();

  Socket.async_read_some(
      asio::buffer(ReadBuffer),
      [this, Self](std::error_code ErrorCode, size_t Length)
      {
        if (!ErrorCode)
        {
          std::println("Client: {}", std::string(reinterpret_cast<char *>(ReadBuffer.data()), Length));
        }
        Write(Length);
      });
}

void session::Write(size_t Length)
{
  auto Self = shared_from_this();
  asio::async_write(Socket, asio::buffer(WriteBuffer, Length),
                    [this, Self](asio::error_code ErrorCode, size_t)
                    {
                      if (!ErrorCode)
                      {
                        Read();
                      }
                    });
}
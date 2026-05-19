#include "Server.hpp"

#include <print>
#include <string>

const size_t KILOBYTE = 1024;
const size_t PACKAGE_SIZE = KILOBYTE * 4;

server::server() : Acceptor(IoContext, tcp::endpoint(tcp::v4(), 8080)), Parser{}
{
}

void server::Run()
{
  using asio::ip::tcp;
  try
  {
    while (true)
    {
      Listen();
    }
  }
  catch (std::exception &Ex)
  {
    std::println("Error: {}", Ex.what());
  }
}

void server::Listen()
{
  tcp::socket Socket(IoContext);
  Acceptor.accept(Socket);

  std::array<char, PACKAGE_SIZE> Buffer;
  size_t Length = Socket.read_some(asio::buffer(Buffer));

  std::string Request(Buffer.data(), Length);
  std::println("Request:\n{}", Request);

  std::string Body{"Hello HTTP!"};
  std::string Response = "HTTP/1.1 200 OK\r\n"
                         "Content-Type: text/plain\r\n"
                         "Content-Length: " +
                         std::to_string(Body.size()) +
                         "\r\n"
                         "\r\n" +
                         Body;
  asio::write(Socket, asio::buffer(Buffer));
}

#include <print>
#include <span>
#include <string>
#include <vector>

#include <asio.hpp>

using asio::ip::tcp;

std::vector<std::string> Requests = {{"GET / HTTP/1.1\r\n"
                                      "Host: localhost\r\n"
                                      "Connection: close\r\n"
                                      "\r\n"},

                                     {"POST /users HTTP/1.1\r\n"
                                      "Host: example.com Content -\r\n"
                                      "Type: application/x-www-form-urlencoded\r\n"
                                      "Content-Length : 49\r\n"
                                      "\r\n"
                                      "name=FirstName+LastName&email=bsmth%40example.com\r\n"
                                      "\r\n"},

                                     {"GET /index.html HTTP/1.1\r\n"
                                      "Host: www.example.com\r\n"
                                      "User-Agent: Mozilla/5.0\r\n"
                                      "Accept: text/html\r\n"
                                      "\r\n"},

                                     {"GET /hello.htm HTTP/1.1\r\n"
                                      "User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\n"
                                      "Host: www.tutorialspoint.com\r\n"
                                      "Accept-Language: en-us\r\n"
                                      "Accept-Encoding: gzip, deflate\r\n"
                                      "Connection: Keep-Alive\r\n"
                                      "\r\n"},

                                     {"POST /cgi-bin/process.cgi HTTP/1.1\r\n"
                                      "User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\n"
                                      "Host: www.tutorialspoint.com\r\n"
                                      "Content-Type: text/xml; charset=utf-8\r\n"
                                      "Content-Length: 60\r\n"
                                      "Accept-Language: en-us\r\n"
                                      "Accept-Encoding: gzip, deflate\r\n"
                                      "Connection: Keep-Alive\r\n"
                                      "\r\n"
                                      "first=Zara&last=Ali\r\n"
                                      "\r\n"}};

std::array<char, 4096> Buffer;

void GrabSomeData(tcp::socket &Socket)
{
  Socket.async_read_some(asio::buffer(Buffer.data(), Buffer.size()),
                         [&](asio::error_code ErrorCode, std::size_t Length)
                         {
                           if (!ErrorCode)
                           {
                             std::println("\n\nRead {} bytes\n\n", Length);
                             std::println("{}", std::string(Buffer.data(), Buffer.size()));

                             GrabSomeData(Socket);
                           }
                         });
}

int main(int argc, char *argv[])
{
  std::vector<std::string> Argv;
  for (const auto El : std::span(argv, argc))
  {
    Argv.push_back(El);
    std::println("{}", El);
  }

  asio::error_code ErrorCode;

  asio::io_context IoContext;
  asio::io_context::work IdleWork(IoContext);

  std::thread Thread = std::thread([&]() { IoContext.run(); });

  tcp::resolver Resolver(IoContext);
  auto Endpoints = Resolver.resolve("127.0.0.1", "80");
  tcp::socket Socket(IoContext);

  asio::connect(Socket, Endpoints);

  if (!ErrorCode)
  {
    std::println("Connected");
  }
  else
  {
    std::println("Failed to connect to address: {}", ErrorCode.message());
  }

  if (Socket.is_open())
  {
    GrabSomeData(Socket);

    std::string Request = std::format("GET / HTTP/1.1\r\n"
                                      "Host: {}\n"
                                      "Connection: close\r\n\r\n",
                                      Argv[1]);
    Socket.write_some(asio::buffer(Request), ErrorCode);

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(20000ms);

    IoContext.stop();
    if (Thread.joinable())
    {
      Thread.join();
    }
  }

  return EXIT_SUCCESS;
}
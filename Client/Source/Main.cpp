#include <print>
#include <span>
#include <string>
#include <vector>

#include <asio.hpp>

using std::string_literals::operator""s;

using asio::ip::tcp;

std::vector<std::string> Requests = {
    {"GET / HTTP/1.1\r\n"s
     "Host: localhost\r\n"s
     "Connection: close\r\n"s
     "\r\n"s},

    {"POST /users HTTP/1.1\r\n"s
     "Host: example.com\r\n"s
     "Content-Type: application/x-www-form-urlencoded\r\n"s
     "Content-Length: 49\r\n"s
     "\r\n"s
     "name=FirstName+LastName&email=bsmth%40example.com\r\n"s
     "\r\n"s},

    {"GET /index.html HTTP/1.1\r\n"s
     "Host: www.example.com\r\n"s
     "User-Agent: Mozilla/5.0\r\n"s
     "Accept: text/html\r\n"s
     "\r\n"s},

    {"GET /hello.htm HTTP/1.1\r\n"s
     "User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\n"s
     "Host: www.tutorialspoint.com\r\n"s
     "Accept-Language: en-us\r\n"s
     "Accept-Encoding: gzip, deflate\r\n"s
     "Connection: Keep-Alive\r\n"s
     "\r\n"s},

    {"POST /cgi-bin/process.cgi HTTP/1.1\r\n"s
     "User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\n"s
     "Host: www.tutorialspoint.com\r\n"s
     "Content-Type: text/xml; charset = utf - 8\r\n"s
     "Content-Length: 60\r\n"s
     "Accept-Language: en-us\r\n"s
     "Accept-Encoding: gzip, deflate\r\n"s
     "Connection: Keep-Alive\r\n"s
     "\r\n"s
     "first=Zara&last=Ali\r\n"s
     "\r\n"s}};

std::array<char, 4096> Buffer;

void GrabSomeData(tcp::socket &Socket)
{
  Socket.async_read_some(asio::buffer(Buffer.data(), Buffer.size()),
                         [&](asio::error_code ErrorCode, std::size_t Length) {
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

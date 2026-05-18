#include <print>
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
                                      "Accept: text/html\r\n"},

                                     {"GET /hello.htm HTTP/1.1\r\n"
                                      "User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\n"
                                      "Host: www.tutorialspoint.com\r\n"
                                      "Accept-Language: en-us\r\n"
                                      "Accept-Encoding: gzip, deflate\r\n"
                                      "Connection: Keep-Alive\r\n"},

                                     {"POST /cgi-bin/process.cgi HTTP/1.1\r\n"
                                      "User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\n"
                                      "Host: www.tutorialspoint.com\r\n"
                                      "Content-Type: text/xml; charset=utf-8\r\n"
                                      "Content-Length: 60\r\n"
                                      "Accept-Language: en-us\r\n"
                                      "Accept-Encoding: gzip, deflate\r\n"
                                      "Connection: Keep-Alive\r\n"
                                      "\r\n"
                                      "first=Zara&last=Ali\r\n"}};

int main()
{
  try
  {
    asio::io_context IoContext;

    tcp::resolver Resolver(IoContext);

    auto Endpoints = Resolver.resolve("localhost", "8080");
    tcp::socket Socket(IoContext);

    asio::connect(Socket, Endpoints);

    for (std::string R : Requests)
    {
      asio::write(Socket, asio::buffer(R));

      std::array<char, 4096> Buffer;
      std::string Response;
      std::error_code ErrorCode;

      while (true)
      {
        size_t Length = Socket.read_some(asio::buffer(Buffer), ErrorCode);
        if (ErrorCode == asio::error::eof)
        {
          break;
        }

        if (ErrorCode)
        {
          throw asio::system_error(ErrorCode);
        }

        Response.append(Buffer.data(), Length);
      }

      std::println("Response:\n{}", Response);
    }
  }
  catch (std::exception &Ex)
  {
    std::println("Error: {}", Ex.what());
  }

  return EXIT_SUCCESS;
}
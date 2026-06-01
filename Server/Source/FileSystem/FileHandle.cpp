#include "FileHandle.hpp"

asio::awaitable<void> SendHttpFile(asio::ip::tcp::socket &socket,
                                   asio::stream_file &file,
                                   std::string file_name) {
  std::string header =
      "HTTP/1.1 200 OK\r\n"
      "Content-Type: application/octet-stream\r\n"
      "Content-Length: " +
      std::to_string(file.size()) +
      "\r\n"
      "Connection: close\r\n"
      "\r\n";

  try {
    co_await asio::async_write(socket, asio::buffer(header),
                               asio::use_awaitable);

    std::vector<char> chunk_buffer(kBytesAmount);

    for (;;) {
      std::size_t bytes_read = co_await file.async_read_some(
          asio::buffer(chunk_buffer), asio::use_awaitable);

      if (bytes_read == 0) {
        break;
      }

      co_await asio::async_write(socket,
                                 asio::buffer(chunk_buffer, bytes_read));
    }
  } catch (const std::system_error &error) {
  }
}

asio::awaitable<void> SendHttpChunked(asio::ip::tcp::socket &socket,
                                      asio::stream_file &file) {
  std::string header =
      "HTTP/1.1 200 OK\r\n"
      "Content-Type: video/mp4\r\n"
      "Transfer-Encoding: chunked\r\n"
      "\r\n";

  co_await asio::async_write(socket, asio::buffer(header), asio::use_awaitable);

  std::vector<char> file_buffer(kBytesAmount);

  for (;;) {
    std::size_t bytes_read = co_await file.async_read_some(
        asio::buffer(file_buffer), asio::use_awaitable);
    if (bytes_read == 0) {
      break;
    }

    std::string chunk_header = std::format("{:x}\r\n", bytes_read);
    std::string chunk_footer = "\r\n";

    std::array<asio::const_buffer, 3> buffers = {
        asio::buffer(chunk_header), asio::buffer(file_buffer, bytes_read),
        asio::buffer(chunk_footer)};

    co_await asio::async_write(socket, buffers, asio::use_awaitable);
  }

  std::string final_chunk = "0\r\n\r\n";
  co_await asio::async_write(socket, asio::buffer(final_chunk),
                             asio::use_awaitable);
}

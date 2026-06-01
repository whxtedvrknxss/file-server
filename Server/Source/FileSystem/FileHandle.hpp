#pragma once

#include <asio.hpp>
#include <vector>

#include "HTTP/Common.hpp"

class FileHandler {
 private:
  asio::stream_file file_;
};

using tcp = asio::ip::tcp;

constexpr static std::size_t kKilobyte = 1024;
constexpr static std::size_t kBytesAmount = 64 * kKilobyte;

asio::awaitable<void> SendHttpFile(tcp::socket &socket, asio::stream_file &file,
                                   std::string file_name);
asio::awaitable<void> SendHttpChunked(tcp::socket &socket,
                                      asio::stream_file &file);

template <http::AsyncReadStream Source, http::AsyncWriteStream Destination>
asio::awaitable<std::size_t> AsyncPipeStream(Source &source,
                                             Destination &destination) {
  std::vector<char> buffer(kBytesAmount);
  std::size_t total_bytes{};

  for (;;) {
    std::size_t bytes_read = co_await source.async_read_some(
        asio::buffer(buffer), asio::use_awaitable);

    if (bytes_read == 0) [[unlikely]] {
      break;
    }

    co_await asio::async_write(destination, asio::buffer(buffer, bytes_read),
                               asio::use_awaitable);

    total_bytes += bytes_read;
  }

  co_return total_bytes;
}

namespace fs {

template <typename DataProcessor>
asio::awaitable<void> StreamFileChunks(asio::stream_file &File,
                                       DataProcessor &process_chunk) {
  std::vector<char> FileBuffer(kBytesAmount);

  for (;;) {
    std::size_t BytesRead = co_await AsyncPipeStream(File, FileBuffer);

    if (BytesRead == 0) {
      break;
    }

    co_await ProcessChunk(asio::buffer(FileBuffer, BytesRead));
  }
}

}  // namespace fs

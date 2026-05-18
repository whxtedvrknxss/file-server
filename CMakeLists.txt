cmake_minimum_required(VERSION 3.24)
project(FileServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SERVER_EXECUTABLE_NAME Server)
set(CLIENT_EXECUTABLE_NAME Client)

add_subdirectory(Server)
add_subdirectory(Client)

include(FetchContent)

FetchContent_Declare(
  asio
  GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
  GIT_TAG asio-1-30-2
)
FetchContent_MakeAvailable(asio)
add_library(AsioInterface INTERFACE)
target_include_directories(AsioInterface INTERFACE 
  "${asio_SOURCE_DIR}/asio/include"
)
target_compile_definitions(AsioInterface INTERFACE 
  ASIO_STANDALONE
  _WIN32_WINNT=0x0601
)

FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.12.0
)
FetchContent_MakeAvailable(json)

find_package(OpenSSL REQUIRED)
add_library(OpenSSLInterface INTERFACE)

target_link_libraries(OpenSSLInterface INTERFACE
  OpenSSL::SSL
  OpenSSL::Crypto
)

set_target_properties(Server PROPERTIES 
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Output/Server"
)
set_target_properties(Server PROPERTIES 
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Output/Client"
)


add_executable(${SERVER_EXECUTABLE_NAME})

target_sources(${SERVER_EXECUTABLE_NAME} PRIVATE 
  "Source/Main.cpp"
  "Source/HTTP/Parser.hpp"
  "Source/HTTP/Parser.cpp"
)

target_link_libraries(${SERVER_EXECUTABLE_NAME} PRIVATE 
  AsioInterface
  OpenSSLInterface
  nlohmann_json::nlohmann_json
)

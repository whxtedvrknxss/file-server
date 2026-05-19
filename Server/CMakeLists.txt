add_executable(${SERVER_EXECUTABLE_NAME})

target_sources(${SERVER_EXECUTABLE_NAME} PRIVATE 
  "Source/Main.cpp"
  "Source/HTTP/Parser.hpp"
  "Source/HTTP/Parser.cpp"
)

target_include_directories(${SERVER_EXECUTABLE_NAME} PRIVATE 
  "Source"
)

target_link_libraries(${SERVER_EXECUTABLE_NAME} PRIVATE 
  llhttpInterface
  asioInterface
  OpenSSLInterface
  nlohmann_json::nlohmann_json
)

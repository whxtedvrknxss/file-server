add_executable(${SERVER_EXECUTABLE_NAME})

target_sources(${SERVER_EXECUTABLE_NAME} PRIVATE 
  "Source/Main.cpp"
  "Source/Core/Server.cpp"
  "Source/HTTP/RequestParser.cpp"
  "Source/HTTP/RequestBuilder.cpp"
  "Source/HTTP/Session.cpp"
)

target_include_directories(${SERVER_EXECUTABLE_NAME} PRIVATE 
  "Source"
)

target_link_libraries(${SERVER_EXECUTABLE_NAME} PRIVATE 
  llhttpInterface
  asioInterface
  OpenSSLInterface
  jsonInterface
)

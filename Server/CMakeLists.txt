add_executable(${SERVER_EXECUTABLE_NAME})

target_sources(${SERVER_EXECUTABLE_NAME} PRIVATE 
  "Source/Main.cpp"
  "Source/Core/Server.cpp"
  "Source/Core/Router.cpp"
  "Source/HTTP/Request/Builder.cpp"
  "Source/HTTP/Request/Parser.cpp"
  "Source/HTTP/ResponseBuilder.cpp"
  "Source/Connection/SessionManager.cpp"
  "Source/Connection/Session.cpp"
  "Source/Work/ThreadPool.cpp"
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

add_executable(${CLIENT_EXECUTABLE_NAME})

target_sources(${CLIENT_EXECUTABLE_NAME} PRIVATE 
  "Source/Main.cpp"
)

target_link_libraries(${CLIENT_EXECUTABLE_NAME} PRIVATE 
  asioInterface
  OpenSSLInterface
  nlohmann_json::nlohmann_json
)


#include "Core/Server.hpp"

int main(int argc, char *argv[]) {
  Server server(8080);
  server.Run();

  return EXIT_SUCCESS;
}

#include "Core/Server.hpp"

int main(int argc, char *argv[])
{
  server Server(8080);
  Server.Run();

  return EXIT_SUCCESS;
}

#include "game.hpp"
#include <cstdlib>

int main(int argc, char **argv) {
  Game game;
  if (!game.Init()) {
    SDL_Log("Initialization failed!");
    return EXIT_FAILURE;
  }
  game.Run();
  game.Shutdown();
  return EXIT_SUCCESS;
}
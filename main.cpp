#include "gameclass.h"

int main () {
  GameClass game;

  if (game.HasFailed())
    return 1;

  game.Run();

  return 0;
}

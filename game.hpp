#pragma once

#include <SDL2/SDL.h>

struct Vector2 {
    float x, y;
};

class Game {
public:
  Game();
  bool Init();
  void Shutdown();
  void Run();

private:
  void ProcessInput();
  void UpdateInput();
  void GenerateOuput();

  SDL_Window *mWindow;
  SDL_Renderer *mRenderer;
  bool mRunning;
  Vector2 mScreenSize;
  Vector2 mPosition, mBallPos;
  Uint32 mTicksCount;
  float mPaddleSpeed;
  Vector2 mBallSpeed;
};

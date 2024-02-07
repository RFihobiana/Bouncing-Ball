#include "game.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <cmath>

Game::Game() : mWindow(nullptr), mRenderer(nullptr), mRunning(true) {
  mScreenSize = (Vector2){1024.0f, 680.0f};
  mPosition = (Vector2){mScreenSize.x / 2.0f, mScreenSize.y - 20};
  mBallPos = (Vector2){mScreenSize.x / 2.0f, 20.0f};
  mPaddleSpeed = 0.0f;
  mBallSpeed = (Vector2){200.0f, 200.0f};
}

bool Game::Init() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    SDL_Log("SDL could not initialized! %s", SDL_GetError());
    return false;
  }
  if (!(mWindow = SDL_CreateWindow("Bouncing ball", SDL_WINDOWPOS_UNDEFINED,
                                   SDL_WINDOWPOS_UNDEFINED, mScreenSize.x,
                                   mScreenSize.y, SDL_WINDOW_SHOWN))) {
    SDL_Log("Failed to create window. %s", SDL_GetError());
    return false;
  }
  if (!(mRenderer = SDL_CreateRenderer(mWindow, -1,
                                       SDL_RENDERER_ACCELERATED |
                                           SDL_RENDERER_PRESENTVSYNC))) {
    SDL_Log("Failed to create renderer. %s", SDL_GetError());
    return false;
  }

  mTicksCount = SDL_GetTicks();
  return true;
}

void Game::Shutdown() {
  SDL_DestroyRenderer(mRenderer);
  SDL_DestroyWindow(mWindow);
  SDL_Quit();

  mRenderer = nullptr;
  mWindow = nullptr;
}

void Game::Run() {
  while (mRunning) {
    ProcessInput();
    UpdateInput();
    GenerateOuput();
  }
}

void Game::ProcessInput() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT)
      mRunning = false;
  }

  const Uint8 *state = SDL_GetKeyboardState(nullptr);
  if (state[SDL_SCANCODE_ESCAPE])
    mRunning = false;

  mPaddleSpeed = 0.0f;
  const float SPEED = 500.0f;
  if (state[SDL_SCANCODE_LEFT])
    mPaddleSpeed = -SPEED;
  if (state[SDL_SCANCODE_RIGHT])
    mPaddleSpeed = SPEED;
}

void Game::UpdateInput() {
  while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
    ;
  float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
  if (deltaTime >= 0.05f)
    deltaTime = 0.05f;
  mTicksCount = SDL_GetTicks();

  // Handle paddle position
  mPosition.x += mPaddleSpeed * deltaTime;
  if (mPosition.x + 100.0f >= mScreenSize.x)
    mPosition.x = mScreenSize.x - 100.0f;
  else if (mPosition.x <= 100.0f)
    mPosition.x = 100.0f;

  // Ball position
  if ((mBallPos.x - 20.0f <= 0.0f && mBallSpeed.x < 0.0f) ||
      (mBallPos.x + 20.0f >= mScreenSize.x && mBallSpeed.x > 0.0f))
    mBallSpeed.x *= -1.0f;

  if (mBallPos.y - 20.0f <= 0.0f && mBallSpeed.y < 0.0f)
    mBallSpeed.y *= -1.0f;
  mBallPos.x += mBallSpeed.x * deltaTime;
  mBallPos.y += mBallSpeed.y * deltaTime;

  // Is the paddle touch the ball?
  if ((abs(mPosition.x - mBallPos.x) <= 120.0f && abs(mPosition.y - mBallPos.y) <= 40.0f) &&
      mBallPos.y > mScreenSize.y - 2.0f*40.0f && mBallSpeed.y > 0.0f)
    mBallSpeed.y *= -1.0f;

  // Ball is outter => Die
  if (mBallPos.y + 20.0f >= mScreenSize.y && mBallSpeed.y > 0.0f)
    mRunning = false;
}

void Game::GenerateOuput() {
  SDL_SetRenderDrawColor(mRenderer, 77, 84, 76, 50);
  SDL_RenderClear(mRenderer);

  SDL_Rect paddle = {static_cast<int>(mPosition.x - 200.0 / 2.0f),
                     static_cast<int>(mPosition.y - 40.0 / 2.0f), 200, 40};
  SDL_SetRenderDrawColor(mRenderer, 93, 196, 231, 255);
  SDL_RenderFillRect(mRenderer, &paddle);

  // BALL
  SDL_Rect ball = {static_cast<int>(mBallPos.x - 40.0f / 2.0f),
                   static_cast<int>(mBallPos.y - 40.0f / 2.0f), 40, 40};
  SDL_SetRenderDrawColor(mRenderer, 224, 240, 161, 128);
  SDL_RenderFillRect(mRenderer, &ball);

  SDL_RenderPresent(mRenderer);
}

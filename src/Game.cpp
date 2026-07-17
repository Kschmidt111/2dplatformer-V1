#include "Game.h"
#include "Camera.h"
#include "Level.h"
#include "Player.h"
#include <SDL.h>
#include <algorithm>

bool Game::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        return false;

    window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (window == nullptr)
        return false;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
        return false;

    running = true;
    return true;
}

void Game::run()
{
    Uint32 lastTime = SDL_GetTicks();
    Player player;
    Level level;
    Camera camera;

    while (running)
    {
        input.update();
        if (input.shouldQuit())
            running = false;

        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
        deltaTime = std::clamp(deltaTime, 0.0f, 0.05f); 
        player.update(deltaTime, input, level.platforms());
        camera.x = std::clamp(player.x - 400.0f, 0.0f, 35600.0f);
        SDL_SetRenderDrawColor(renderer, 30, 30, 40, 255);
        SDL_RenderClear(renderer);
        level.draw(renderer, camera);
        player.draw(renderer, camera);
        SDL_RenderPresent(renderer);
    }
}

void Game::clean()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

#include "Game.h"
#include <SDL.h>

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

void Game::run(){
    Uint32 lastTime = SDL_GetTicks();
    while (running){
        SDL_Event event;
     
        while (SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                      
                running = false;
            }
            if(event.type == SDL_KEYDOWN){
                if(event.key.keysym.sym == SDLK_ESCAPE){
                    running = false;
                }
            }
                     
        }
        SDL_SetRenderDrawColor(renderer, 30, 30, 40, 255);
        //every frame we clear the renderer
        SDL_RenderClear(renderer);
        //every frame we render the scene
        SDL_RenderPresent(renderer);
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
        
    }
}


void Game::clean(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
#pragma once

struct SDL_Window;
struct SDL_Renderer;

#include "Input.h"

class Game
{
    public:
        bool init();
        void run();
        void clean();

    private:
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
        bool running = false;
        Input input;
};
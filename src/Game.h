#pragma once

struct SDL_Window;

class Game
{
    public:
        bool init();
        void run();
        void clean();


    private:
        SDL_Window* window = nullptr;
};
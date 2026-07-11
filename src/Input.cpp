#include "Input.h"

void Input::update()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_KEYDOWN)
        {
            keys[event.key.keysym.scancode] = true;

            if (event.key.keysym.sym == SDLK_ESCAPE)
                quit = true;
        }

        if (event.type == SDL_KEYUP)
            keys[event.key.keysym.scancode] = false;

        if (event.type == SDL_QUIT)
            quit = true;

        // -------- DEBUG INPUT (comment out this block) --------
        // if (event.type == SDL_KEYDOWN)
        //     SDL_Log("DEBUG key DOWN: %s", SDL_GetKeyName(event.key.keysym.sym));
        // else if (event.type == SDL_KEYUP)
        //     SDL_Log("DEBUG key UP:   %s", SDL_GetKeyName(event.key.keysym.sym));
        // -------- DEBUG INPUT END --------
    }
}
bool Input::isKeyDown(SDL_Keycode key) const
{
    return keys[SDL_GetScancodeFromKey(key)];
}

bool Input::shouldQuit() const
{
    return quit;
}

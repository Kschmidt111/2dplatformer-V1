#pragma once

#include <SDL.h>

class Input
{
public:
    void update();
    bool shouldQuit() const;
    bool isKeyDown(SDL_Keycode key) const;
    bool isKeyPressed(SDL_Keycode key) const;

private:
    bool keys[SDL_NUM_SCANCODES] = {};
    bool pressed[SDL_NUM_SCANCODES] = {};
    bool quit = false;
};

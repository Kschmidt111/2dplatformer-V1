#pragma once

#include "Input.h"

struct SDL_Renderer;

class Camera;

class Player
{
public:
    void update(float deltaTime, const Input& input);
    void draw(SDL_Renderer* renderer, const Camera& camera) const;

private:
    float x{100};
    float y{400};
    float width{32};
    float height{32};
    float speed{200};
};

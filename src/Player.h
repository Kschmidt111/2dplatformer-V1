#pragma once
#include <string.h>
#include "Input.h"
#include "Level.h"
#include <vector>

struct SDL_Renderer;

class Camera;

class Player
{
public:
    void update(float deltaTime, const Input& input, const std::vector<Platform>& platforms);
    void draw(SDL_Renderer* renderer, const Camera& camera) const;
    void overlap(Platform platforms);
    bool leftHit(const Platform* platform);
private:
    float x{100};
    float y{400};
    float width{32};
    float height{32};
    float speed{200};
    float vy{0};
    bool onGround{false};
};

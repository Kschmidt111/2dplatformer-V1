#pragma once

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
    float x{100};
    float y{400};

private:
    void applyHorizontalInput(float deltaTime, const Input& input);
    void applyJump(float deltaTime, const Input& input);
    void resolveHorizontal(const std::vector<Platform>& platforms);
    void applyGravity(float deltaTime);
    void resolveVertical(const std::vector<Platform>& platforms);

    void overlapX(const Platform& platform);
    void overlapY(const Platform& platform);
    bool overlaps(const Platform& platform) const;
    void respawn();

    int jumpsLeft{2};
    float width{32};
    float height{32};
    float speed{200};
    float vy{0};
    bool onGround{false};
    float jumpBuffer{0};
};

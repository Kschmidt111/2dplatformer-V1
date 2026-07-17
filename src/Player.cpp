#include "Player.h"

#include "Camera.h"
#include "SDL_keycode.h"
#include "SDL_render.h"
#include <SDL.h>
#include "Level.h"

const float gravity{800};
const float jumpForce{-500};
const float jumpBufferTime{0.12f};

void Player::update(float deltaTime, const Input& input, const std::vector<Platform>& platforms)
{
    applyHorizontalInput(deltaTime, input);
    applyJump(deltaTime, input);
    resolveHorizontal(platforms);
    applyGravity(deltaTime);
    resolveVertical(platforms);
}

void Player::applyHorizontalInput(float deltaTime, const Input& input)
{
    if (input.isKeyDown(SDLK_LEFT) || input.isKeyDown(SDLK_a))
        x -= speed * deltaTime;

    if (input.isKeyDown(SDLK_RIGHT) || input.isKeyDown(SDLK_d))
        x += speed * deltaTime;
}

void Player::applyJump(float deltaTime, const Input& input)
{
    if (input.isKeyPressed(SDLK_SPACE))
        jumpBuffer = jumpBufferTime;

    if (jumpBuffer > 0.0f)
        jumpBuffer -= deltaTime;

    if (jumpBuffer > 0.0f && jumpsLeft > 0)
    {
        vy = jumpForce;
        onGround = false;
        jumpBuffer = 0.0f;
        jumpsLeft -= 1;
    }
}

void Player::resolveHorizontal(const std::vector<Platform>& platforms)
{
    for (const Platform& platform : platforms)
        overlapX(platform);
}

void Player::applyGravity(float deltaTime)
{
    vy += gravity * deltaTime;
    y += vy * deltaTime;
}

void Player::resolveVertical(const std::vector<Platform>& platforms)
{
    onGround = false;
    for (const Platform& platform : platforms)
        overlapY(platform);
}

void Player::draw(SDL_Renderer* renderer, const Camera& camera) const
{
    SDL_Rect rect;
    rect.x = static_cast<int>(x - camera.x);
    rect.y = static_cast<int>(y - camera.y);
    rect.w = static_cast<int>(width);
    rect.h = static_cast<int>(height);
    SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);
    SDL_RenderFillRect(renderer, &rect);
}

bool Player::overlaps(const Platform& platform) const
{
    return x < platform.x + platform.width &&
           x + width > platform.x &&
           y < platform.y + platform.height &&
           y + height > platform.y;
}

void Player::respawn(){
    x = 100;
    y = 400;
}

void Player::overlapY(const Platform& platform)
{
    if (!overlaps(platform))
        return;
    if(platform.field == Lava){
        respawn();
    }
    if (vy >= 0)
    {
        y = platform.y - height;
        vy = 0;
        onGround = true;
        jumpsLeft = 2;
    }
    else
    {
        y = platform.y + platform.height;
        vy = 0;
    }
}

void Player::overlapX(const Platform& platform)
{
    if (!overlaps(platform))
        return;

    float playerCenter{x + width / 2};
    float platformCenter{platform.x + platform.width / 2};
    if (playerCenter < platformCenter)
        x = platform.x - width;
    else
        x = platform.x + platform.width;
}

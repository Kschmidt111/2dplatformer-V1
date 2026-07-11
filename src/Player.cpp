#include "Player.h"

#include "Camera.h"
#include "SDL_render.h"
#include <SDL.h>

void Player::update(float deltaTime, const Input& input)
{
    if (input.isKeyDown(SDLK_LEFT) || input.isKeyDown(SDLK_a))
        x -= speed * deltaTime;

    if (input.isKeyDown(SDLK_RIGHT) || input.isKeyDown(SDLK_d))
        x += speed * deltaTime;
}

void Player::draw(SDL_Renderer* renderer, const Camera& camera) const
{

    SDL_Rect rect;
    rect.x = static_cast<int>(x - camera.x);
    rect.y = static_cast<int>(y - camera.y);
    rect.w = static_cast<int>(width);
    rect.h = static_cast<int>(height);
    SDL_SetRenderDrawColor(renderer,  255,  100,  100,  255);
    SDL_RenderFillRect(renderer, &rect);

}

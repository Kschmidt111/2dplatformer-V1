#include "Player.h"

#include "Camera.h"
#include "SDL_keycode.h"
#include "SDL_render.h"
#include <SDL.h>
#include <cstddef>
#include <vector>
#include "Level.h"

const float gravity{800};
const float groundY{500};
const float jumpForce{-500};
void Player::update(float deltaTime, const Input& input, const std::vector<Platform>& platforms)
{
    if (input.isKeyDown(SDLK_LEFT) || input.isKeyDown(SDLK_a)){
        x -= speed * deltaTime;
    }
    if (input.isKeyDown(SDLK_RIGHT) || input.isKeyDown(SDLK_d)){
        x += speed * deltaTime;
    }

    for (const Platform& platform : platforms){
        overlap(platform);
    }

    if (input.isKeyPressed(SDLK_SPACE) && onGround){
        vy = jumpForce;
    }





    vy += gravity * deltaTime; 
    y += vy * deltaTime;
    int feet = y + height;
    if(feet >= groundY){
        y = groundY - height;
        vy=0;
        onGround = true;
    }
    else{
        onGround = false;
    }

    
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

void Player::overlap(Platform platform) 
{

        if (x < platform.x + platform.width &&
            x + width > platform.x &&
            y < platform.y + platform.height &&
            y + height > platform.y){

                float playerCenter{(x + width)/2};
                float platformCenter{(platform.x + platform.width)/2};
                if(playerCenter < platformCenter){
                    x = platform.x - width;
                }
                else{
                    x = platform.x + platform.width;
                }

            }
            
    

}

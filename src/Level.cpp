#include "Level.h"
#include "Camera.h"
#include "SDL_render.h"


Level::Level(){

    platformList.push_back({0, 500, 800, 100});
    platformList.push_back({200, 400, 120, 20});
    platformList.push_back({400, 320, 120, 20});
    platformList.push_back({600, 250, 120, 20});
}

const std::vector<Platform>& Level::platforms() const{
    return platformList;
}

void Level::draw(SDL_Renderer* renderer, const Camera& camera) const
{
    for (const Platform& platform : platformList)
    {
        SDL_Rect rect;
        rect.x = static_cast<int>(platform.x - camera.x);
        rect.y = static_cast<int>(platform.y - camera.y);
        rect.w = static_cast<int>(platform.width);
        rect.h = static_cast<int>(platform.height);
        SDL_SetRenderDrawColor(renderer, 100, 200, 100, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
}
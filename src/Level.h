#pragma once

#include <vector>


struct SDL_Renderer;
class Camera;
struct Platform
{
    float x{};
    float y{};
    float width{};
    float height{};
};

class Level
{
public:
    void draw(SDL_Renderer* renderer, const Camera& camera) const;
    const std::vector<Platform>& platforms() const;
    Level();
private:
    std::vector<Platform> platformList;
};

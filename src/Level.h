#pragma once

#include <vector>


struct SDL_Renderer;
class Camera;

enum Field {
    Normal,
    Lava,
    Gold
};

struct Platform
{
    float x{};
    float y{};
    float width{};
    float height{};
    Field field{Normal};
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

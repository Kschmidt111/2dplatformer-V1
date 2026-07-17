#include "Level.h"
#include "Camera.h"
#include "SDL_render.h"
#include <algorithm>

namespace
{
    // Player: speed 200, jumpForce -500, gravity 800
    // Single jump ~156px high. Keep consecutive height changes under ~130 for double jump.
    // Tune these to change the run feel:
    constexpr float kLevelEndX = 18000.0f;
    constexpr float kLavaY = 500.0f;
    constexpr float kPlatformW = 110.0f;
    constexpr float kPlatformH = 20.0f;

    constexpr float kStepX = 300.0f;          // average left-edge spacing
    constexpr float kStepXVariance = 90.0f;   // spacing varies by ± this

    // Height variety (bigger = wilder Y, but capped so jumps stay fair)
    constexpr float kStepY = 90.0f;           // average |height change| between platforms
    constexpr float kStepYVariance = 50.0f;   // how much that change varies (±)
    constexpr float kMaxHeightDelta = 130.0f; // hard cap per step (double-jump safe)

    constexpr float kBaseY = 340.0f;          // starting height of the trail
    constexpr float kMinY = 160.0f;           // highest platforms (smaller y = higher on screen)
    constexpr float kMaxY = 450.0f;           // lowest platforms (above lava at 500)

    // Deterministic 0..1 from index (same layout every run)
    float noise01(int i, int salt)
    {
        unsigned n = static_cast<unsigned>(i * 374761393 + salt * 668265263);
        n = (n ^ (n >> 13)) * 1274126177u;
        return static_cast<float>(n % 10000u) / 10000.0f;
    }

}

Level::Level()
{
    // Safe spawn pad (not lava)
    platformList.push_back({0, kLavaY, 220, 100, Normal});

    // Lava floor for the rest of the run
    platformList.push_back({200, kLavaY, kLevelEndX - 200, 100, Lava});

    // Intro staircase onto the lava run
    platformList.push_back({200, 420, 120, kPlatformH, Normal});
    platformList.push_back({380, 360, 120, kPlatformH, Normal});
    platformList.push_back({560, 300, 120, kPlatformH, Normal});

    // Main trail: pick a random height in range each step, but never jump more than kMaxHeightDelta
    float x = 760.0f;
    float y = kBaseY;
    int i = 0;

    while (x + kPlatformW < kLevelEndX - 400.0f)
    {
        platformList.push_back({x, y, kPlatformW, kPlatformH, Normal});

        float xGap = kStepX + (noise01(i, 1) * 2.0f - 1.0f) * kStepXVariance;

        // Random target anywhere in the height band → lots of different Y values
        float targetY = kMinY + noise01(i, 2) * (kMaxY - kMinY);

        // Move toward that target, but only by a fair jump amount
        float desired = targetY - y;
        float magnitude = kStepY + (noise01(i, 3) * 2.0f - 1.0f) * kStepYVariance;
        magnitude = std::clamp(magnitude, 20.0f, kMaxHeightDelta);

        float delta = desired;
        if (delta > magnitude) delta = magnitude;
        if (delta < -magnitude) delta = -magnitude;

        y = std::clamp(y + delta, kMinY, kMaxY);

        x += xGap;
        ++i;
    }

    // Final approach + gold goal
    platformList.push_back({kLevelEndX - 380, 360, 140, kPlatformH, Normal});
    platformList.push_back({kLevelEndX - 200, 320, 160, kPlatformH, Gold});
}

const std::vector<Platform>& Level::platforms() const
{
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

        if (platform.field == Lava)
            SDL_SetRenderDrawColor(renderer, 220, 80, 30, 255);
        else if (platform.field == Gold)
            SDL_SetRenderDrawColor(renderer, 230, 200, 40, 255);
        else
            SDL_SetRenderDrawColor(renderer, 100, 200, 100, 255);

        SDL_RenderFillRect(renderer, &rect);
    }
}

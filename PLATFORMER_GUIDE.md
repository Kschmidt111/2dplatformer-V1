# Simple 2D Platformer in C++ with SDL2

A step-by-step guide for building a small platformer from scratch. This guide tells you **what to do and why** — it does not include code. When you want code for a specific step, ask for it.

---

## What You Are Building

A minimal but complete game:

- A window with a fixed-size viewport (e.g. 800×600)
- A player rectangle that moves left/right and jumps
- Gravity and ground collision
- A few static platforms
- A simple camera that follows the player horizontally
- Restart on death (falling off the map)
- Clean shutdown (no leaks, no crashes on exit)

That is enough to feel like a real platformer. Everything else (sprites, sound, enemies, levels) comes later.

---

## Prerequisites

You should already be comfortable with:

- Variables, functions, classes, references, pointers
- `std::vector`, basic file I/O
- Separating code into `.h` / `.cpp` files
- Compiling and linking a multi-file project

You do **not** need to know SDL2 beforehand.

---

## Tools and Libraries

| Tool | Purpose |
|------|---------|
| **C++ compiler** | MSVC (Visual Studio), MinGW, or Clang |
| **CMake** (recommended) | Build system — keeps the project portable |
| **SDL2** | Window, input, timing, 2D rendering |
| **SDL2_image** (optional for now) | Load PNG/BMP later — skip until Step 8 |

Install SDL2 for your platform and make sure you can compile and run the official SDL2 "Hello World" example before continuing. If that does not work, fix your setup first — every later step depends on it.

---

## Project Structure (Target)

Organize early so you do not rewrite everything later:

```
2dplatformer/
├── CMakeLists.txt
├── src/
│   ├── main.cpp          # Entry point only
│   ├── Game.h / Game.cpp # Owns the loop, creates/destroys subsystems
│   ├── Input.h / Input.cpp
│   ├── Player.h / Player.cpp
│   ├── Level.h / Level.cpp
│   └── Camera.h / Camera.cpp
└── assets/               # Empty for now; add images later
```

**Rule:** `main.cpp` creates a `Game` object and calls `run()`. Nothing else lives in `main`.

---

## Core Concepts Before You Start

### The Game Loop

Every frame, in this order:

1. **Process input** — read keyboard state
2. **Update** — move entities, apply physics, check collisions (use delta time)
3. **Render** — clear screen, draw everything, present to screen
4. **Repeat** until the player quits

Use a fixed target frame rate (60 FPS) or cap delta time so physics do not break on slow machines.

### Delta Time

Store the time since the last frame in seconds (a `float`). Multiply all movement and physics by delta time so behavior is consistent regardless of frame rate.

### Separation of Update and Draw

Update logic must not call rendering functions. Draw logic must not change game state. This keeps bugs localized and makes the code easier to extend.

### Coordinate System

SDL2 uses a top-left origin: X increases to the right, Y increases **downward**. "Up" means **decreasing** Y. Keep this in mind for gravity (positive Y velocity = falling).

---

## Step 1 — Window and Game Loop

**Goal:** Open a window, run a loop, close cleanly on Escape or window close.

**Do this:**

1. Create the project with CMake and link SDL2.
2. Write a `Game` class with `init()`, `run()`, and `cleanup()`.
3. In `init()`, create an SDL window and renderer.
4. In `run()`, loop until a quit flag is set:
   - Poll SDL events; set quit on `SDL_QUIT` or Escape key.
   - Clear the renderer to a solid color.
   - Present the renderer.
   - Measure delta time between frames.
5. In `cleanup()`, destroy renderer, window, and call `SDL_Quit()`.

**Verify:** Window opens, background color renders, Escape closes without errors or leaks.

**Common mistakes:** Forgetting `SDL_Quit()`, not handling `SDL_QUIT`, creating the renderer before the window.

---

## Step 2 — Input System

**Goal:** Track which keys are held this frame.

**Do this:**

1. Create an `Input` class (or namespace) with `update()` and query methods like `isKeyDown(key)`.
2. In `update()`, poll events. On key down/up, update an internal array or map of key states.
3. Also handle quit events here and expose a `shouldQuit()` method.
4. Call `Input::update()` once at the start of each frame, before game logic.

**Verify:** Print or display debug info showing key states change when you press and release keys.

**Why a separate system:** Game logic reads input state; it does not poll SDL events directly. This keeps `Player` free of SDL event code.

---

## Step 3 — Player Entity (No Physics Yet)

**Goal:** A visible rectangle that moves when you hold arrow keys or A/D.

**Do this:**

1. Create a `Player` class with position (`float x, y`), size (`width, height`), and speed.
2. Add `update(deltaTime, input)` — read horizontal input, update `x` by `speed * deltaTime`.
3. Add `draw(renderer, camera)` — draw a filled rectangle at the player's world position (camera offset applied later; use raw position for now).
4. Instantiate one `Player` in `Game` and call update/draw each frame.

**Verify:** Rectangle moves left/right smoothly. It does not fall or jump yet.

**Use rectangles first.** Do not load sprites until movement feels good.

---

## Step 4 — Gravity and Jumping

**Goal:** Player falls, lands on an implicit floor, and can jump.

**Do this:**

1. Add vertical velocity (`vy`) and a gravity constant (positive value — remember Y-down).
2. Each frame: apply gravity to `vy`, then add `vy * deltaTime` to `y`.
3. Define a temporary ground line (e.g. `y = 500`). If the player's feet would go below it, clamp `y` and set `vy = 0`.
4. On jump key press (not hold — use a "key pressed this frame" check), if on ground, set `vy` to a negative jump force.
5. Track `onGround` with a boolean updated during floor collision.

**Verify:** Player falls, lands, jumps once per press, cannot double-jump (for now).

**Tune by feel:** Adjust gravity, jump force, and move speed until movement feels responsive.

---

## Step 5 — Level and Platforms

**Goal:** Replace the single floor line with multiple platform rectangles.

**Do this:**

1. Create a `Platform` struct: `x, y, width, height`.
2. Create a `Level` class that holds a `std::vector<Platform>`.
3. Hard-code 4–6 platforms in `Level` constructor or a `load()` method (tile maps come later).
4. Add `Level::draw(renderer, camera)` to draw all platforms.
5. Move floor collision from `Player` into a dedicated collision pass against the level.

**Verify:** Player lands on each platform, falls between gaps, cannot stand in mid-air.

---

## Step 6 — Axis-Aligned Bounding Box (AABB) Collision

**Goal:** Correct collision response for a platformer.

**Do this:**

1. Implement AABB overlap test between player bounds and each platform.
2. Resolve collision on **both axes separately** (standard approach):
   - Apply horizontal movement, then check horizontal overlaps and push player out.
   - Apply vertical movement, then check vertical overlaps and push player out.
   - If resolved vertically from above, set `onGround = true` and `vy = 0`.
3. Run collision after integrating velocity, not before.

**Verify:** Player does not sink into platforms, does not jitter on edges, lands cleanly on top.

**Common mistakes:** Resolving both axes at once (causes corner snagging), applying collision before movement integration.

---

## Step 7 — Camera

**Goal:** View scrolls horizontally so the player stays near the center.

**Do this:**

1. Create a `Camera` with `x, y` world offset (start with `y = 0` — no vertical scroll yet).
2. Each frame, set camera `x` so the player is centered: `camera.x = player.x - screenWidth / 2`.
3. Optionally clamp camera so it does not show empty space beyond level bounds.
4. Pass camera to all `draw()` methods. Subtract camera offset from world positions before drawing.
5. **Do not** subtract camera from collision logic — collision stays in world space.

**Verify:** Player moves through a wider level; camera follows smoothly; platforms draw at correct screen positions.

---

## Step 8 — Death and Restart

**Goal:** Falling off the bottom resets the player.

**Do this:**

1. Define a kill zone (e.g. `y > levelHeight + margin`).
2. When triggered, reset player position and velocity to spawn point.
3. Optionally flash the screen or print "respawned" for debug.

**Verify:** Jump off the map, player respawns at start, game continues.

---

## Step 9 — Polish the Loop

**Goal:** Stable, readable foundation before adding assets.

**Do this:**

1. Cap delta time (e.g. max 0.05s) to avoid physics explosions after a pause.
2. Use consistent naming and const-correctness.
3. Make sure every `SDL_Create*` has a matching destroy in `cleanup()`.
4. Run in Debug and Release — fix any warnings.

**Verify:** Tab out and back in — no huge jump in player position. No memory leaks reported by your tools (if available).

---

## Step 10 — Sprites (Optional Next Step)

Only after Steps 1–9 work with colored rectangles:

1. Add SDL2_image, load a PNG for player and platforms.
2. Replace `SDL_RenderFillRect` with `SDL_RenderCopy` using source/dest rectangles.
3. Keep the same hitbox sizes even if the sprite is larger — gameplay uses AABB, not pixel-perfect art.

Ask for code when you reach this step if you want help with texture loading.

---

## Step 11 — Ideas After the Core Works

Pick one at a time:

| Feature | What to learn |
|---------|---------------|
| Tile map from file | File parsing, grid indexing |
| Animation | Sprite sheets, frame timers |
| Moving platforms | Platform velocity added to player on contact |
| Collectibles | Trigger volumes, score counter |
| Enemies | Simple patrol AI, damage on overlap |
| Audio | SDL2_mixer, sound on jump/land |
| State machine | Title screen, playing, game over |

---

## Debugging Tips

- **Player falls through floor:** Collision runs before position update, or ground check uses wrong edge (use feet = `y + height`).
- **Jitter on platform edge:** Separate axis resolution order wrong; try horizontal first, then vertical.
- **Movement too fast/slow:** Forgot to multiply by delta time.
- **Nothing draws:** Forgot `SDL_RenderPresent`, or camera offset pushes everything off-screen.
- **Black screen:** Renderer clear color is black and nothing is drawn on top — draw platforms/player before present.

Draw world-space debug rectangles (hitboxes) in a bright color while tuning collision. Remove them when done.

---

## Order of Implementation Checklist

Use this as your progress tracker:

- [ ] Step 1 — Window and game loop
- [ ] Step 2 — Input system
- [ ] Step 3 — Player horizontal movement
- [ ] Step 4 — Gravity and jump
- [ ] Step 5 — Level with platforms
- [ ] Step 6 — AABB collision
- [ ] Step 7 — Camera
- [ ] Step 8 — Death and restart
- [ ] Step 9 — Loop polish
- [ ] Step 10 — Sprites (optional)

---

## How to Use This Guide

1. Complete one step fully before moving to the next.
2. If something breaks, fix it before continuing — later steps assume earlier ones work.
3. When you want implementation help, ask: *"Write the code for Step 4"* (or whichever step you are on).
4. Keep the scope small. A fun rectangle platformer beats an unfinished sprite game.

Good luck — start with Step 1.

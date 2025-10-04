#include <entt.hpp>
#include "SDL3/SDL.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include "components/Transform.hpp"
#include "components/Renderable.hpp"
#include "components/Velocity.hpp"
#include "components/Acceleration.hpp"
#include "components/Force.hpp"
#include "components/Mass.hpp"
#include "systems/RenderSystem.hpp"
#include "systems/ForceSystem.hpp"
#include "systems/PhysicsSystem.hpp"

static constexpr double TARGET_FPS = 60.0;
static constexpr double TARGET_FRAME_TIME = 1.0 / TARGET_FPS;

static void capFrameRate(Uint64 frameStart, double targetFrameTime) {
    Uint64 frameEnd = SDL_GetPerformanceCounter();
    Uint64 elapsedTicks = frameEnd - frameStart;
    Uint64 freq = SDL_GetPerformanceFrequency();

    // converte ticks -> nanosegundos (evita float enquanto possível)
    Uint64 elapsedNs = (elapsedTicks * 1000000000ull) / freq;
    Uint64 nsPerFrame = static_cast<Uint64>(targetFrameTime * 1e9);

    if (elapsedNs < nsPerFrame) {
        SDL_DelayNS(nsPerFrame - elapsedNs);
    }
}

auto init(SDL_Window **window, SDL_Renderer **renderer) -> bool
{   
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                    "Falha ao inicializar SDL_VIDEO: %s",
                    SDL_GetError());
        return false;
    }

    bool success = SDL_CreateWindowAndRenderer(
        "Lunar", 256, 224, SDL_WINDOW_RESIZABLE, window, renderer);

    if (success == false)
    {
        SDL_Log("ERROR!");
        return false;
    }

    SDL_Log("START - GAME");
    return true;
}

int main(int /*argc*/, char * /*argv*/[])
{
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

    entt::registry registry;

    if (init(&window, &renderer) == false)
        return -1;

    auto square = registry.create();
    registry.emplace<Transform>(square, (256 / 2 - 8), 0, 16.0, 32.0);
    registry.emplace<Renderable>(square, SDL_Color{255, 255, 255, 255});
    registry.emplace<Velocity>(square, 0.0f, 0.0f);
    registry.emplace<Acceleration>(square, 0.0f, 0.0f);
    registry.emplace<Force>(square, 0.0, 9.81);
    registry.emplace<Mass>(square, 1.0f);

    RenderSystem renderSystem(renderer);
    ForceSystem forceSystem;
    PhysicsSystem physicsSystem;

    bool should_run{true};
    SDL_Event e;

    // usa performance counter pra deltaTime preciso
    Uint64 now = SDL_GetPerformanceCounter();
    Uint64 last = now;
    double deltaTime = 0.0;

    while (should_run)
    {
        last = now;
        now = SDL_GetPerformanceCounter();
        deltaTime = (double)(now - last) / (double)SDL_GetPerformanceFrequency(); // segundos

        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT)
                should_run = false;

            if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_ESCAPE)
                should_run = false;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        forceSystem.update(registry);
        physicsSystem.update(registry, static_cast<float>(deltaTime));
        renderSystem.update(registry);

        SDL_RenderPresent(renderer);
        capFrameRate(now, TARGET_FRAME_TIME);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

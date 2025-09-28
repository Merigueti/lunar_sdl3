#include <entt.hpp>
#include "SDL3/SDL.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include "components/Transform.hpp"
#include "components/Renderable.hpp"
#include "systems/RenderSystem.cpp"


auto init(SDL_Window **window, SDL_Renderer **renderer) -> auto
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
    {
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
    SDL_Window *window;
    SDL_Renderer *renderer;

    entt::registry registry;

    if (init(&window, &renderer) == false)
    {
        return -1;
    }

    auto square = registry.create();
    registry.emplace<Transform>(square, (256/2 - 8), (224/2 - 16), 16.0, 32.0);
    registry.emplace<Renderable>(square, SDL_Color{255, 255, 255, 255});

    RenderSystem renderSystem(renderer);

    bool should_run{true};
    SDL_Event e;

    while (should_run)
    {
        if (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT)
            {
                should_run = false;
            }
            if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_ESCAPE)
            {
                should_run = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        renderSystem.update(registry);

        SDL_RenderPresent(renderer);
    }

    // **************************************************************

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

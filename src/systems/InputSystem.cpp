#include "systems/InputSystem.hpp"
#include "components/InputState.hpp"

void InputSystem::update(entt::registry& registry) {
    auto &ctx = registry.ctx();
    InputState *inputPtr = nullptr;

    if (ctx.contains<InputState>()) {
        inputPtr = &ctx.get<InputState>();
    } else {
        inputPtr = &ctx.emplace<InputState>();
    }

    auto &input = *inputPtr;

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_EVENT_QUIT) {
            ctx.insert_or_assign<bool>(true);
        }

        if (e.type == SDL_EVENT_KEY_DOWN) {
            switch (e.key.key) {
                case SDLK_W:    input.up = true; break;
                case SDLK_S:  input.down = true; break;
                case SDLK_A:  input.left = true; break;
                case SDLK_D: input.right = true; break;
                case SDLK_ESCAPE:
                    ctx.insert_or_assign<bool>(true);
                    break;
            }
        }

        if (e.type == SDL_EVENT_KEY_UP) {
            switch (e.key.key) {
                case SDLK_W:    input.up = false; break;
                case SDLK_S:  input.down = false; break;
                case SDLK_A:  input.left = false; break;
                case SDLK_D: input.right = false; break;
            }
        }
    }
}

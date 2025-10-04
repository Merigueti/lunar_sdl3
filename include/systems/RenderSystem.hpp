#pragma once

#include <entt.hpp>
#include <SDL3/SDL.h>

class RenderSystem {
public:
    explicit RenderSystem(SDL_Renderer* renderer);
    void update(entt::registry& registry);

private:
    SDL_Renderer* m_renderer;
};

// src/systems/RenderSystem.cpp
#include <entt.hpp>
#include <SDL3/SDL.h>
#include "components/Transform.hpp"
#include "components/Renderable.hpp"

class RenderSystem {
public:
    RenderSystem(SDL_Renderer* renderer) : m_renderer(renderer) {}

    void update(entt::registry& registry) {
        auto view = registry.view<Transform, Renderable>();
        for (auto entity : view) {
            auto& transform = view.get<Transform>(entity);
            auto& renderable = view.get<Renderable>(entity);

            SDL_FRect rect{transform.x, transform.y, transform.w, transform.h};
            SDL_SetRenderDrawColor(m_renderer,
                                   renderable.color.r,
                                   renderable.color.g,
                                   renderable.color.b,
                                   renderable.color.a);
            SDL_RenderFillRect(m_renderer, &rect);
        }
    }

private:
    SDL_Renderer* m_renderer;
};


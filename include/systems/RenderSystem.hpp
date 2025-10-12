#pragma once

#include <entt.hpp>
#include <SDL3/SDL.h>
#include "core/AssetManager.hpp"
#include "components/Sprite.hpp"

class RenderSystem {
public:
    explicit RenderSystem(SDL_Renderer* renderer, AssetManager* assetManager);
    void update(entt::registry& registry);

private:
    SDL_Renderer* m_renderer;
    AssetManager* m_assetManager;
};

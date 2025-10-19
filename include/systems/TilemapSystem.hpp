#ifndef TILEMAP_SYSTEM_HPP
#define TILEMAP_SYSTEM_HPP

#include <entt/entt.hpp>
#include "components/Tilemap.hpp"
#include "components/TileLayer.hpp"
#include "core/AssetManager.hpp"
#include <SDL3/SDL.h>

class TilemapSystem {
public:
    TilemapSystem(SDL_Renderer* renderer, AssetManager* assets);
    void render(entt::registry& registry);

private:
    SDL_Renderer* renderer;
    AssetManager* assets;
};

#endif

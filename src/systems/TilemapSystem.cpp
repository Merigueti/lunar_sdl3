// systems/TilemapSystem.cpp
#include "systems/TilemapSystem.hpp"
#include "components/Tilemap.hpp"
#include <SDL3/SDL.h>
#include "systems/TilemapSystem.hpp"

TilemapSystem::TilemapSystem(SDL_Renderer* renderer, AssetManager* assets) 
    : renderer(renderer), assets(assets) {}

void TilemapSystem::render(entt::registry& registry) {
    auto view = registry.view<Tilemap>();

    for (auto entity : view) {
        const auto& map = view.get<Tilemap>(entity);
        SDL_Texture* texture = assets->getTexture(map.tilesetTextureId);
        if (!texture) continue;

        // Get texture dimensions to calculate tiles per row
        float textureWidth, textureHeight;
        SDL_GetTextureSize(texture, &textureWidth, &textureHeight);
        int tilesPerRow = textureWidth / map.tileWidth;

        for (const auto& layer : map.layers) {
            for (int y = 0; y < layer.height; ++y) {
                for (int x = 0; x < layer.width; ++x) {
                    int tileId = layer.tiles[y * layer.width + x];
                    if (tileId == 0) continue; // 0 = empty in Tiled

                    tileId -= 1; // Adjust index (Tiled starts at 1)

                    int srcX = (tileId % tilesPerRow) * map.tileWidth;
                    int srcY = (tileId / tilesPerRow) * map.tileHeight;

                    SDL_FRect srcRect = { 
                        static_cast<float>(srcX), 
                        static_cast<float>(srcY), 
                        static_cast<float>(map.tileWidth), 
                        static_cast<float>(map.tileHeight) 
                    };
                    SDL_FRect dstRect = { 
                        static_cast<float>(x * map.tileWidth), 
                        static_cast<float>(y * map.tileHeight),
                        static_cast<float>(map.tileWidth), 
                        static_cast<float>(map.tileHeight) 
                    };

                    SDL_RenderTexture(renderer, texture, &srcRect, &dstRect);
                }
            }
        }
    }
}
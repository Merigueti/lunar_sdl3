#include "systems/TileCollisionSystem.hpp"

bool TileCollisionSystem::checkCollision(const Tilemap& map, float left, float top, float right, float bottom) {
    int startTileX = static_cast<int>(left)   / map.tileWidth;
    int endTileX   = static_cast<int>(right)  / map.tileWidth;
    int startTileY = static_cast<int>(top)    / map.tileHeight;
    int endTileY   = static_cast<int>(bottom) / map.tileHeight;

    for (const auto& layer : map.layers) {
        for (int ty = startTileY; ty <= endTileY; ++ty) {
            for (int tx = startTileX; tx <= endTileX; ++tx) {
                if (tx < 0 || ty < 0 || tx >= layer.width || ty >= layer.height)
                    continue;

                int idx = ty * layer.width + tx;
                if (layer.tiles[idx] != 0) {
                    return true; // há colisão
                }
            }
        }
    }

    return false;
}

void TileCollisionSystem::update(entt::registry& registry) {
    auto mapView = registry.view<Tilemap>();
    if (mapView.empty()) return;

    const auto& map = mapView.get<Tilemap>(*mapView.begin());
    auto view = registry.view<Collider, Transform>();

    for (auto entity : view) {
        auto& collider = view.get<Collider>(entity);
        auto& transform = view.get<Transform>(entity);

        float oldX = transform.previousX;
        float oldY = transform.previousY;

        float newX = transform.x;
        float newY = transform.y;

        // -------- Eixo X --------
        {
            float left   = newX;
            float right  = newX + collider.w;
            float top    = oldY;
            float bottom = oldY + collider.h;

            if (checkCollision(map, left, top, right, bottom)) {
                if (newX > oldX) {
                    newX = (static_cast<int>(right) / map.tileWidth) * map.tileWidth - collider.w - 0.1f;
                } else {
                    newX = (static_cast<int>(left) / map.tileWidth + 1) * map.tileWidth + 0.1f;
                }
            }
        }

        // -------- Eixo Y --------
        {
            float left   = newX;
            float right  = newX + collider.w;
            float top    = newY;
            float bottom = newY + collider.h;

            if (checkCollision(map, left, top, right, bottom)) {
                // Corrige Y: volta até não colidir
                if (newY > oldY) {
                    // movendo para baixo
                    newY = (static_cast<int>(bottom) / map.tileHeight) * map.tileHeight - collider.h - 0.1f;
                } else {
                    // movendo para cima
                    newY = (static_cast<int>(top) / map.tileHeight + 1) * map.tileHeight + 0.1f;
                }
            }
        }

        transform.x = newX;
        transform.y = newY;
    }
}

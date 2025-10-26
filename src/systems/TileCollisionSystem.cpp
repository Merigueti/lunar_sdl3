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

        collider.trigger_left_to_right = false;
        collider.trigger_right_to_left = false;
        collider.trigger_up_to_down = false;
        collider.trigger_down_to_up = false;

        // Calcular as coordenadas da caixa de colisão considerando o offset
        float colliderLeft = newX + collider.x;
        float colliderTop = newY + collider.y;
        float colliderRight = colliderLeft + collider.w;
        float colliderBottom = colliderTop + collider.h;

        float oldColliderLeft = oldX + collider.x;
        float oldColliderTop = oldY + collider.y;
        float oldColliderRight = oldColliderLeft + collider.w;
        float oldColliderBottom = oldColliderTop + collider.h;

        // -------- Eixo X --------
        {
            float left   = colliderLeft;
            float right  = colliderRight;
            float top    = oldColliderTop;    // Usar Y antigo para verificação X
            float bottom = oldColliderBottom; // Usar Y antigo para verificação X

            if (checkCollision(map, left, top, right, bottom)) {
                if (newX > oldX) {
                    collider.trigger_left_to_right = true;
                    if(collider.solid) {
                        // Corrigir a posição X baseado na colisão
                        float collisionRight = (static_cast<int>(right) / map.tileWidth) * map.tileWidth;
                        newX = collisionRight - collider.w - collider.x - 0.1f;
                    }
                } else {
                    collider.trigger_right_to_left = true;
                    if(collider.solid) {
                        // Corrigir a posição X baseado na colisão
                        float collisionLeft = (static_cast<int>(left) / map.tileWidth + 1) * map.tileWidth;
                        newX = collisionLeft - collider.x + 0.1f;
                    }
                }
            }
        }

        // -------- Eixo Y --------
        {
            // Atualizar coordenadas da caixa de colisão com o X corrigido
            colliderLeft = newX + collider.x;
            colliderTop = newY + collider.y;
            colliderRight = colliderLeft + collider.w;
            colliderBottom = colliderTop + collider.h;

            float left   = colliderLeft;
            float right  = colliderRight;
            float top    = colliderTop;
            float bottom = colliderBottom;

            if (checkCollision(map, left, top, right, bottom)) {
                // Corrige Y: volta até não colidir
                if (newY > oldY) {
                    collider.trigger_up_to_down = true;
                    if(collider.solid) {
                        float collisionBottom = (static_cast<int>(bottom) / map.tileHeight) * map.tileHeight;
                        newY = collisionBottom - collider.h - collider.y - 0.1f;
                    }
                } else {
                    collider.trigger_down_to_up = true;
                    if(collider.solid) {
                        float collisionTop = (static_cast<int>(top) / map.tileHeight + 1) * map.tileHeight;
                        newY = collisionTop - collider.y + 0.1f;
                    }
                }
            }
        }
        
        transform.x = newX;
        transform.y = newY;
    }
}
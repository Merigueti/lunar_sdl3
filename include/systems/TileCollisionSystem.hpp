#pragma once
#include <entt/entt.hpp>
#include <algorithm>
#include <cmath>
#include "SDL3/SDL.h"
#include "systems/TilemapSystem.hpp"
#include "components/Transform.hpp"
#include "components/TileLayer.hpp"
#include "components/Tilemap.hpp"
#include "components/Collider.hpp"
#include "components/Acceleration.hpp"

class TileCollisionSystem {
public:
    void update(entt::registry& registry);
    bool checkCollision(const Tilemap& map, float left, float top, float right, float bottom);
};

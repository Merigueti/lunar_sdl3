#pragma once
#include <entt/entt.hpp>

class GravitySystem {
public:
    void update(entt::registry& registry, float deltaTime);
};

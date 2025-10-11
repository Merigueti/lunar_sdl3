#pragma once
#include <entt.hpp>

class GravitySystem {
public:
    void update(entt::registry& registry, float deltaTime);
};

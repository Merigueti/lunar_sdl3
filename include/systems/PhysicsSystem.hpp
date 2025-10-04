#pragma once
#include <entt.hpp>

class PhysicsSystem {
public:
    void update(entt::registry& registry, float deltaTime);
};

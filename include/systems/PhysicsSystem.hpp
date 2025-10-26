#pragma once
#include <entt/entt.hpp>
#include "components/Acceleration.hpp"
#include "components/Transform.hpp"
#include "components/Velocity.hpp"
#include "components/Friction.hpp"
#include "components/Collider.hpp"

class PhysicsSystem {
public:
    void update(entt::registry& registry, float deltaTime);
};

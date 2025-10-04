#include "systems/PhysicsSystem.hpp"
#include "components/Acceleration.hpp"
#include "components/Transform.hpp"
#include "components/Velocity.hpp"
#include "SDL3/SDL.h"

constexpr float PIXELS_PER_METER = 32.0f;

void PhysicsSystem::update(entt::registry &registry, float deltaTime)
{
    auto view = registry.view<Transform, Velocity, Acceleration>();
    for (auto entity : view)
    {
        auto &t = view.get<Transform>(entity);
        auto &v = view.get<Velocity>(entity);
        auto &a = view.get<Acceleration>(entity);

        v.dx += a.ax * deltaTime;
        v.dy += a.ay * deltaTime;

        t.x += static_cast<int>(v.dx * deltaTime * PIXELS_PER_METER);
        t.y += static_cast<int>(v.dy * deltaTime * PIXELS_PER_METER);
    }
}

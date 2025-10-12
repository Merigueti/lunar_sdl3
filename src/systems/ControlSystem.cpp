#include "systems/ControlSystem.hpp"
#include "components/InputState.hpp"
#include "components/Force.hpp"
#include "components/Mass.hpp"
#include "components/Transform.hpp"
#include "components/Velocity.hpp"
#include "components/Friction.hpp"
#include <cmath>

void ControlSystem::update(entt::registry &registry)
{
    auto &input = registry.ctx().get<InputState>();
    auto view = registry.view<Transform, Velocity, Force, Mass, Friction>();

    for (auto entity : view)
    {
        auto &force = view.get<Force>(entity);
        auto &mass = view.get<Mass>(entity);
        auto &fric = view.get<Friction>(entity);

        force.x = 0.0f;
        force.y = 0.0f;
        fric.value = 0.9f;

        const float MOVEMENT_FORCE = 50.0f;

        float dirX = 0.0f;
        float dirY = 0.0f;

        if (input.up)    dirY -= 1.0f;
        if (input.down)  dirY += 1.0f;
        if (input.left)  dirX -= 1.0f;
        if (input.right) dirX += 1.0f;

        float magnitude = std::sqrt(dirX * dirX + dirY * dirY);
        if (magnitude > 0.0f)
        {
            dirX /= magnitude;
            dirY /= magnitude;

            force.x = dirX * MOVEMENT_FORCE * mass.value;
            force.y = dirY * MOVEMENT_FORCE * mass.value;

            fric.value = 1.0f;
        }
    }
}

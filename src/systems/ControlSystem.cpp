#include "systems/ControlSystem.hpp"
#include "components/InputState.hpp"
#include "components/Force.hpp"
#include "components/Mass.hpp"
#include "components/Transform.hpp"
#include "components/Velocity.hpp"
#include "components/Friction.hpp"

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

        // Aplica forças baseadas no input (valores ajustáveis)
        const float MOVEMENT_FORCE = 50.0f; // Ajuste conforme necessário

        if (input.up)
        {
            force.y -= MOVEMENT_FORCE * mass.value;
            fric.value = 1.0f;
        }
        if (input.down)
        {
            force.y += MOVEMENT_FORCE * mass.value;
            fric.value = 1.0f;
        }
        if (input.left)
        {
            force.x -= MOVEMENT_FORCE * mass.value;
            fric.value = 1.0f;
        }
        if (input.right)
        {
            force.x += MOVEMENT_FORCE * mass.value;
            fric.value = 1.0f;
        }
    }
}

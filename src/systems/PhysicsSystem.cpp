#include "systems/PhysicsSystem.hpp"
#include "components/Acceleration.hpp"
#include "components/Transform.hpp"
#include "components/Velocity.hpp"
#include "components/Friction.hpp"

constexpr float PIXELS_PER_METER = 32.0f;

void PhysicsSystem::update(entt::registry &registry, float deltaTime)
{
    auto view = registry.view<Transform, Velocity, Acceleration, Friction>();

    for (auto entity : view)
    {
        auto &vel = view.get<Velocity>(entity);
        auto &accel = view.get<Acceleration>(entity);
        auto &fric = view.get<Friction>(entity);

        // Integração: v = v0 + a * dt
        vel.x += accel.x * deltaTime * PIXELS_PER_METER;
        vel.y += accel.y * deltaTime * PIXELS_PER_METER;

        // LIMITE DE VELOCIDADE MÁXIMA (evita velocidades infinitas)
        const float MAX_SPEED = 500.0f; // Ajuste conforme necessário
        float currentSpeed = sqrt(vel.x * vel.x + vel.y * vel.y);
        if (currentSpeed > MAX_SPEED)
        {
            vel.x = (vel.x / currentSpeed) * MAX_SPEED;
            vel.y = (vel.y / currentSpeed) * MAX_SPEED;
        }

        // Aplica atrito/suavização (reduz velocidade gradualmente)
        vel.x *= fric.value;           
        vel.y *= fric.value;
        if (fabs(vel.x) < 0.1f)
            vel.x = 0.0f;
        if (fabs(vel.y) < 0.1f)
            vel.y = 0.0f;

        auto &transform = view.get<Transform>(entity);
        transform.previousX = transform.x;
        transform.previousY = transform.y;
        transform.x += vel.x * deltaTime;
        transform.y += vel.y * deltaTime;
    }
}
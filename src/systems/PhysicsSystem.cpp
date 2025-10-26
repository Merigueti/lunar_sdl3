#include "systems/PhysicsSystem.hpp"
constexpr float PIXELS_PER_METER = 32.0f;

void PhysicsSystem::update(entt::registry &registry, float deltaTime) {
    auto view = registry.view<Transform, Velocity, Acceleration, Friction, Collider>();

    for (auto entity : view) {
        auto &vel = view.get<Velocity>(entity);
        auto &accel = view.get<Acceleration>(entity);
        auto &fric = view.get<Friction>(entity);
        auto &transform = view.get<Transform>(entity);
        auto &collider = view.get<Collider>(entity);

        // Salva posição anterior
        transform.previousX = transform.x;
        transform.previousY = transform.y;

        // Zera velocidade na direção de colisão usando as flags do Collider
        if (collider.trigger_right_to_left && vel.x < 0) vel.x = 0;
        if (collider.trigger_left_to_right && vel.x > 0) vel.x = 0;
        if (collider.trigger_down_to_up && vel.y < 0) vel.y = 0;
        if (collider.trigger_up_to_down && vel.y > 0) vel.y = 0;

        // Aplica aceleração apenas se não estiver colidindo naquela direção
        if (!(collider.trigger_right_to_left && accel.x < 0) && 
            !(collider.trigger_left_to_right && accel.x > 0)) {
            vel.x += accel.x * deltaTime * PIXELS_PER_METER;
        }
        
        if (!(collider.trigger_down_to_up && accel.y < 0) && 
            !(collider.trigger_up_to_down && accel.y > 0)) {
            vel.y += accel.y * deltaTime * PIXELS_PER_METER;
        }

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

        transform.x += vel.x * deltaTime;
        transform.y += vel.y * deltaTime;
    }
}
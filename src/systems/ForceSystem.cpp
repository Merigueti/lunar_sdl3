#include "systems/ForceSystem.hpp"
#include "components/Force.hpp"
#include "components/Acceleration.hpp"
#include "components/Mass.hpp"
#include <SDL3/SDL.h>


void ForceSystem::update(entt::registry& registry) {
    auto view = registry.view<Force, Acceleration, Mass>();
    for (auto entity : view) {
        auto &force = view.get<Force>(entity);
        auto &acc = view.get<Acceleration>(entity);
        auto &mass = view.get<Mass>(entity);
        
        acc.x = force.x / mass.value;
        acc.y = force.y / mass.value;

        // force.x = 0;
        // force.y = 0;
    }
}

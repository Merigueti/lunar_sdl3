#pragma once
#include <entt/entt.hpp>
#include "SDL3/SDL.h"
#include "components/Force.hpp"
#include "components/Mass.hpp"
#include "systems/AnimationSystem.hpp"

class ControlSystem {
public:
    void update(entt::registry &registry);
};

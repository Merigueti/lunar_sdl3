#pragma once
#include <entt.hpp>
#include "SDL3/SDL.h"
#include "components/Force.hpp"
#include "components/Mass.hpp"

class ControlSystem {
public:
    void update(entt::registry &registry);
};

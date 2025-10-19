#pragma once
#include <entt/entt.hpp>
#include "SDL3/SDL.h"
#include "components/InputState.hpp"

class InputSystem {
public:
    void update(entt::registry& registry);
};

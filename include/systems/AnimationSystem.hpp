#pragma once
#include <string>
#include <entt.hpp>
#include "SDL3/SDL.h"
#include "components/AnimationData.hpp"
#include "components/AnimationSet.hpp"
#include "components/Sprite.hpp"

class AnimationSystem {
public:
    void update(entt::registry& registry, float deltaTime);

    static void setAnimation(entt::registry& registry, entt::entity entity, const std::string& animationName);
    static void stopAnimation(entt::registry& registry, entt::entity entity);
    static void resumeAnimation(entt::registry& registry, entt::entity entity);
};

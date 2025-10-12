#include "systems/AnimationSystem.hpp"

void AnimationSystem::update(entt::registry& registry, float deltaTime) {
    auto view = registry.view<AnimationSet, Sprite>();

    for (auto entity : view) {
        auto &animSet = view.get<AnimationSet>(entity);
        auto &sprite = view.get<Sprite>(entity);

        if (!animSet.playing || animSet.current.empty()) continue;

        auto it = animSet.animations.find(animSet.current);
        if (it == animSet.animations.end()) continue;

        const AnimationData &anim = it->second;

        animSet.accumulator += deltaTime;
        
        if (animSet.accumulator >= anim.frameTime) {
            animSet.accumulator -= anim.frameTime;
            animSet.currentFrame++;
            
            if (animSet.currentFrame > anim.endFrame) {
                if (anim.loop) {
                    animSet.currentFrame = anim.startFrame;
                } else {
                    animSet.currentFrame = anim.endFrame;
                    animSet.playing = false;
                }
            }

            // Sincroniza com o componente Sprite
            sprite.currentFrame = animSet.currentFrame;
        }
    }
}

void AnimationSystem::setAnimation(entt::registry& registry, entt::entity entity, const std::string& animationName) {
    if (auto* animSet = registry.try_get<AnimationSet>(entity)) {
        if (animSet->current == animationName) return;
        
        auto it = animSet->animations.find(animationName);
        if (it == animSet->animations.end()) return;

        animSet->current = animationName;
        animSet->currentFrame = it->second.startFrame;
        animSet->accumulator = 0.0f;
        animSet->playing = true;

        // Atualiza sprite imediatamente
        if (auto* sprite = registry.try_get<Sprite>(entity)) {
            sprite->currentFrame = animSet->currentFrame;
        }
    }
}

void AnimationSystem::stopAnimation(entt::registry& registry, entt::entity entity) {
    if (auto* animSet = registry.try_get<AnimationSet>(entity)) {
        animSet->playing = false;
    }
}

void AnimationSystem::resumeAnimation(entt::registry& registry, entt::entity entity) {
    if (auto* animSet = registry.try_get<AnimationSet>(entity)) {
        animSet->playing = true;
    }
}

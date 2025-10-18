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

            // Avança um frame na horizontal
            animSet.currentFrameX++;

            // Se passou do fim da linha, avança para a próxima
            if (animSet.currentFrameX > anim.endFrameX) {
                animSet.currentFrameX = anim.startFrameX;
                animSet.currentFrameY++;
            }

            // Se passou do fim da animação (última linha)
            if (animSet.currentFrameY > anim.endFrameY) {
                if (anim.loop) {
                    animSet.currentFrameX = anim.startFrameX;
                    animSet.currentFrameY = anim.startFrameY;
                } else {
                    animSet.currentFrameX = anim.endFrameX;
                    animSet.currentFrameY = anim.endFrameY;
                    animSet.playing = false;
                }
            }

            // Atualiza sprite
            sprite.frameX = animSet.currentFrameX;
            sprite.frameY = animSet.currentFrameY;
        }
    }
}

void AnimationSystem::setAnimation(entt::registry& registry, entt::entity entity, const std::string& animationName) {
    if (auto* animSet = registry.try_get<AnimationSet>(entity)) {
        if (animSet->current == animationName) return;

        auto it = animSet->animations.find(animationName);
        if (it == animSet->animations.end()) return;

        const AnimationData& anim = it->second;
        animSet->current = animationName;
        animSet->currentFrameX = anim.startFrameX;
        animSet->currentFrameY = anim.startFrameY;
        animSet->accumulator = 0.0f;
        animSet->playing = true;

        // Atualiza sprite imediatamente
        if (auto* sprite = registry.try_get<Sprite>(entity)) {
            sprite->frameX = animSet->currentFrameX;
            sprite->frameY = animSet->currentFrameY;
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
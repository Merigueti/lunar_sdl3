#include "systems/RenderSystem.hpp"
#include "components/Transform.hpp"
#include "components/Renderable.hpp"

RenderSystem::RenderSystem(SDL_Renderer* renderer, AssetManager* assetManager)
    : m_renderer(renderer), m_assetManager(assetManager) {}

void RenderSystem::update(entt::registry& registry) {
    auto view = registry.view<Transform, Renderable, Sprite>();
    SDL_FRect dstRect;
    SDL_FRect srcRect;
    SDL_FlipMode flip;
    for (auto entity : view) {
        auto& transform = view.get<Transform>(entity);
        const auto &sprite = view.get<Sprite>(entity);

        SDL_Texture* texture = m_assetManager->getTexture(sprite.textureId);
        if (!texture) continue;

        srcRect.x = static_cast<float>(sprite.frameWidth * sprite.currentFrame);
        srcRect.y = 0; // Assume spritesheet em uma linha
        srcRect.w = static_cast<float>(sprite.frameWidth);
        srcRect.h = static_cast<float>(sprite.frameHeight);

        dstRect.x = static_cast<float>(transform.x);
        dstRect.y = static_cast<float>(transform.y);
        dstRect.w = static_cast<float>(transform.w);
        dstRect.h = static_cast<float>(transform.h);

        flip = SDL_FLIP_NONE;
        if (sprite.flipX && sprite.flipY) flip = static_cast<SDL_FlipMode>(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
        else if (sprite.flipX) flip = SDL_FLIP_HORIZONTAL;
        else if (sprite.flipY) flip = SDL_FLIP_VERTICAL;
        SDL_RenderTexture(m_renderer, texture, &srcRect, &dstRect);
    }
}

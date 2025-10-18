#include "systems/RenderSystem.hpp"
#include "components/Transform.hpp"
#include "components/Renderable.hpp"

RenderSystem::RenderSystem(SDL_Renderer* renderer, AssetManager* assetManager)
    : m_renderer(renderer), m_assetManager(assetManager) {}

void RenderSystem::update(entt::registry& registry) {
    auto view = registry.view<Transform, Renderable, Sprite>();
    SDL_FRect dstRect;
    SDL_FRect srcRect;

    for (auto entity : view) {
        auto& transform = view.get<Transform>(entity);
        const auto& sprite = view.get<Sprite>(entity);

        SDL_Texture* texture = m_assetManager->getTexture(sprite.textureId);
        if (!texture) continue;

        // --- cálculo com suporte a múltiplas linhas e colunas ---
        srcRect.x = static_cast<float>(sprite.frameX * sprite.frameWidth);
        srcRect.y = static_cast<float>(sprite.frameY * sprite.frameHeight);
        srcRect.w = static_cast<float>(sprite.frameWidth);
        srcRect.h = static_cast<float>(sprite.frameHeight);

        dstRect.x = static_cast<float>(transform.x);
        dstRect.y = static_cast<float>(transform.y);
        dstRect.w = static_cast<float>(transform.w);
        dstRect.h = static_cast<float>(transform.h);

        // --- SDL3 não tem SDL_FlipMode ---
        // Então aplicamos flip "na mão" alterando o srcRect e dstRect
        bool flipX = sprite.flipX;
        bool flipY = sprite.flipY;

        SDL_FRect flippedSrc = srcRect;

        if (flipX) {
            flippedSrc.x += srcRect.w;
            flippedSrc.w = -srcRect.w;
        }
        if (flipY) {
            flippedSrc.y += srcRect.h;
            flippedSrc.h = -srcRect.h;
        }

        SDL_RenderTexture(m_renderer, texture, &flippedSrc, &dstRect);
    }
}

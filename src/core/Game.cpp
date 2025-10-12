#include "core/Game.hpp"
#include "SDL3/SDL.h"
#include <iostream>

Game::Game()
    : window_(nullptr),
      renderer_(nullptr),
      renderSystem_(nullptr, nullptr),  // será configurado após init
      running_(false) {}

Game::~Game() {
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}

bool Game::init() {
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Falha ao inicializar SDL: %s", SDL_GetError());
        return false;
    }

    if (!SDL_CreateWindowAndRenderer("Lunar", 256, 224, SDL_WINDOW_RESIZABLE,
                                     &window_, &renderer_)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Falha ao criar janela/renderizador: %s", SDL_GetError());
        return false;
    }

    if (!assetManager_.loadTexture(renderer_, "hero", "assets/16x32_Idle.bmp")) {
        SDL_Log("Não foi possível carregar 16x32_Idle.bmp, usando fallback");
    }

    // Reconfigurar render system com asset manager
    
    renderSystem_ = RenderSystem(renderer_, &assetManager_);

    registry_.ctx().emplace<InputState>();
    registry_.ctx().emplace<bool>(false);  // quit flag
    createEntities();

    SDL_Log("START - GAME");
    running_ = true;
    return true;
};

void Game::createEntities() {
    auto square = registry_.create();
    registry_.emplace<Transform>(square, (256 / 2 - 8), 0, 16.0, 32.0);
    registry_.emplace<Color>(square, 255, 255, 255, 255);
    registry_.emplace<Renderable>(square);
    registry_.emplace<Velocity>(square, 0.0f, 0.0f);
    registry_.emplace<Acceleration>(square, 0.0f, 0.0f);
    registry_.emplace<Force>(square, 0.0, 0.0);
    registry_.emplace<Mass>(square, 1.0f);
    registry_.emplace<Friction>(square, 0.0f);

    Sprite sprite;
    sprite.textureId = "hero";
    sprite.frameWidth = 16;
    sprite.frameHeight = 32;
    registry_.emplace<Sprite>(square, sprite);
    setupPlayerAnimations(square);

}

void Game::setupPlayerAnimations(entt::entity player) {
    auto& animSet = registry_.emplace<AnimationSet>(player);
    animSet.animations["idle"] = AnimationData{0, 3, 0.2f, true};
}

void Game::run() {
    Uint64 now = SDL_GetPerformanceCounter();
    Uint64 last = now;

    while (running_) {
        last = now;
        now = SDL_GetPerformanceCounter();
        double deltaTime = static_cast<double>(now - last) /
                           static_cast<double>(SDL_GetPerformanceFrequency());

        processFrame(deltaTime);
        capFrameRate(now);
    }
}

void Game::processFrame(double deltaTime) {
    inputSystem_.update(registry_);
    controlSystem_.update(registry_);

    // verifica sinal de quit
    if (registry_.ctx().get<bool>()) {
        running_ = false;
        return;
    }

    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
    SDL_RenderClear(renderer_);

    forceSystem_.update(registry_);
    physicsSystem_.update(registry_, static_cast<float>(deltaTime));

    auto view = registry_.view<Velocity, AnimationSet>();
    for (auto entity : view) {
        const auto& velocity = view.get<Velocity>(entity);
        auto& animSet = view.get<AnimationSet>(entity);
        AnimationSystem::setAnimation(registry_, entity, "idle");
    }


    animationSystem_.update(registry_, deltaTime);
    renderSystem_.update(registry_);

    SDL_RenderPresent(renderer_);
}

void Game::capFrameRate(Uint64 frameStart) {
    Uint64 frameEnd = SDL_GetPerformanceCounter();
    Uint64 elapsedTicks = frameEnd - frameStart;
    Uint64 freq = SDL_GetPerformanceFrequency();

    Uint64 elapsedNs = (elapsedTicks * 1000000000ull) / freq;
    Uint64 nsPerFrame = static_cast<Uint64>(TARGET_FRAME_TIME * 1e9);

    if (elapsedNs < nsPerFrame) {
        SDL_DelayNS(nsPerFrame - elapsedNs);
    }
}

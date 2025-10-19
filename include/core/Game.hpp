#pragma once

#include <entt/entt.hpp>
#include "SDL3/SDL.h"

// Core
#include "core/AssetManager.hpp"
#include "core/TilemapLoader.hpp"

// Components
#include "components/Acceleration.hpp"
#include "components/AnimationData.hpp"
#include "components/AnimationSet.hpp"
#include "components/Color.hpp"
#include "components/Friction.hpp"
#include "components/Force.hpp"
#include "components/InputState.hpp"
#include "components/Mass.hpp"
#include "components/Renderable.hpp"
#include "components/Sprite.hpp"
#include "components/Transform.hpp"
#include "components/Velocity.hpp"
#include "components/Tilemap.hpp"
#include "components/TileLayer.hpp"
#include "components/Collider.hpp"

// Systems
#include "systems/ControlSystem.hpp"
#include "systems/ForceSystem.hpp"
#include "systems/InputSystem.hpp"
#include "systems/PhysicsSystem.hpp"
#include "systems/RenderSystem.hpp"
#include "systems/AnimationSystem.hpp"
#include "systems/TilemapSystem.hpp"
#include "systems/TileCollisionSystem.hpp"


class Game {
public:
    Game();
    ~Game();

    bool init();
    void run();

private:
    void createEntities(int x, int y);
    void processFrame(double deltaTime);
    void capFrameRate(Uint64 frameStart);
    void setupPlayerAnimations(entt::entity player);
    void createTilemap();

    SDL_Window* window_;
    SDL_Renderer* renderer_;
    entt::registry registry_;
    TilemapSystem m_tilemapSystem;
    entt::entity m_tilemapEntity;
    AssetManager assetManager_;

    RenderSystem renderSystem_;
    ForceSystem forceSystem_;
    PhysicsSystem physicsSystem_;
    ControlSystem controlSystem_;
    InputSystem inputSystem_;
    AnimationSystem animationSystem_;
    TileCollisionSystem tileCollisionSystem;

    bool running_;

    static constexpr double TARGET_FPS = 60.0;
    static constexpr double TARGET_FRAME_TIME = 1.0 / TARGET_FPS;
};

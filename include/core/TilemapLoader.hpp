#ifndef TILEMAP_LOADER_HPP
#define TILEMAP_LOADER_HPP

#include <string>
#include <entt/entt.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include "components/Tilemap.hpp"
#include "components/TileLayer.hpp"
#include "AssetManager.hpp"

using json = nlohmann::json;

class TilemapLoader {
public:
    TilemapLoader(entt::registry& registry, AssetManager& assets)
        : registry(registry), assets(assets) {}

    entt::entity loadFromFile(const std::string& path, const std::string& textureId);

private:
    entt::registry& registry;
    AssetManager& assets;
};

#endif

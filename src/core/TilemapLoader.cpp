#include "core/TilemapLoader.hpp"

entt::entity TilemapLoader::loadFromFile(const std::string& path, const std::string& textureId) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Erro ao abrir tilemap: " + path);
    }

    json data;
    file >> data;

    Tilemap tilemap;
    tilemap.tilesetTextureId = textureId;
    tilemap.tileWidth = data["tilewidth"];
    tilemap.tileHeight = data["tileheight"];
    tilemap.mapWidth = data["width"];
    tilemap.mapHeight = data["height"];

    for (const auto& layerData : data["layers"]) {
        if (layerData["type"] != "tilelayer") continue;

        TileLayer layer;
        layer.name = layerData["name"];
        layer.width = layerData["width"];
        layer.height = layerData["height"];
        layer.tiles = layerData["data"].get<std::vector<int>>();

        tilemap.layers.push_back(std::move(layer));
    }

    entt::entity entity = registry.create();
    registry.emplace<Tilemap>(entity, std::move(tilemap));

    return entity;
}

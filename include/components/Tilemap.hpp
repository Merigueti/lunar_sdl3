#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <string>
#include <vector>
#include "TileLayer.hpp"

struct Tilemap {
    std::string tilesetTextureId;
    int tileWidth = 0;
    int tileHeight = 0;
    int mapWidth = 0;
    int mapHeight = 0;
    std::vector<TileLayer> layers;
};

#endif

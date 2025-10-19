#ifndef TILELAYER_HPP
#define TILELAYER_HPP

#include <string>
#include <vector>

struct TileLayer {
    std::string name;
    int width;
    int height;
    std::vector<int> tiles; // IDs dos tiles
};

#endif
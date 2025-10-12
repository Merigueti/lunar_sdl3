#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <string>

struct Sprite {
    std::string textureId;
    int frameWidth = 0;
    int frameHeight = 0;
    int currentFrame = 0;
    bool flipX = false;
    bool flipY = false;
};

#endif
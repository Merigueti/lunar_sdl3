#ifndef ANIMATION_DATA_HPP
#define ANIMATION_DATA_HPP

#include <string>

struct AnimationData {
    int startFrame = 0;
    int endFrame = 0;
    float frameTime = 0.1f; // segundos por frame
    bool loop = true;
};

#endif // ANIMATION_DATA_HPP
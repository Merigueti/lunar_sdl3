#ifndef ANIMATION_DATA_HPP
#define ANIMATION_DATA_HPP

#include <string>

struct AnimationData {
    int startFrameX = 0;
    int startFrameY = 0;
    int endFrameX = 0;
    int endFrameY = 0;
    float frameTime = 0.1f; // segundos por frame
    bool loop = true;
};

#endif // ANIMATION_DATA_HPP
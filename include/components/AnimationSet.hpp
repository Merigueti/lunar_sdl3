#ifndef ANIMATION_SET_HPP
#define ANIMATION_SET_HPP

#include <unordered_map>
#include <string>
#include "AnimationData.hpp"

struct AnimationSet {
    std::unordered_map<std::string, AnimationData> animations;
    std::string current = "";
    int currentFrameX = 0;
    int currentFrameY = 0;
    float accumulator = 0.0f;
    bool playing = true;
};

#endif
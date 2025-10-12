#ifndef ANIMATION_SET_HPP
#define ANIMATION_SET_HPP

#include <unordered_map>
#include <string>
#include "AnimationData.hpp"

struct AnimationSet
{
    std::unordered_map<std::string, AnimationData> animations;
    std::string current = ""; // nome da animação atual
    int currentFrame = 0;     // frame atual
    float accumulator = 0.0f; // tempo acumulado para troca de frame
    bool playing = true;
};

#endif
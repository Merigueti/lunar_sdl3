#pragma once
#include <cstdint>

struct Collider {
    float w;
    float h;
    uint8_t layer;
    bool solid;
    bool trigger;
};

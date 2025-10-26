#pragma once
#include <cstdint>

struct Collider {
    float x, y; //x, y a partir do transform
    float w, h; //Tamanho da caixa de colisão
    uint8_t layer;
    bool solid = true;
    bool trigger_up_to_down = false;
    bool trigger_down_to_up = false;
    bool trigger_right_to_left = false;
    bool trigger_left_to_right = false;
};

#pragma once

#include <cstdint>

struct InputMouseReport {
    uint8_t buttons; // b[0][0][0][0][0][left: 0 or 1][middle: 0 or 1][right: 0 or 1]
    int8_t dx;
    int8_t dy;
    int8_t wheel;
};
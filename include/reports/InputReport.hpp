#pragma once

#include <cstdint>

struct InputMouseReport {
    uint8_t button_state; // b[0][0][0][0][0][left: 0 or 1][middle: 0 or 1][right: 0 or 1]
    int8_t x_pos;
    int8_t y_pos;
    int8_t wheel_value;
};
#pragma once
#include <cstdint>

#define USAGE_PAGE(x) 0x05, x;
#define USAGE(x) 0x09, x;
#define COLLECTION(x) 0xA1, x;
#define USAGE_MINIMUM(x) 0x19, x;
#define USAGE_MAXIMUM(x) 0x29, x;
#define LOGICAL_MAXIMUM(x) 0x25, x;
#define LOGICAL_MINIMUM(x) 0x15, x;

#define REPORT_COUNT(x) 0x95, x;
#define REPORT_SIZE(x) 0x75, x;

#define INPUT_TYPE(x) 0x81, x;





static const uint8_t mouse_report_descriptor[] = {
    0x05, 0x01,        // Usage Page: Generic Desktop
    0x09, 0x02,        // Usage: Mouse
    0xA1, 0x01,        // Collection: Application

    0x09, 0x01,        //   Usage: Pointer
    0xA1, 0x00,        //   Collection: Physical

    0x05, 0x09,        //     Usage Page: Button
    0x19, 0x01,        //     Usage Minimum: Button 1
    0x29, 0x03,        //     Usage Maximum: Button 3
    0x15, 0x00,        //     Logical Minimum: 0
    0x25, 0x01,        //     Logical Maximum: 1
    0x95, 0x03,        //     Report Count: 3
    0x75, 0x01,        //     Report Size: 1
    0x81, 0x02,        //     Input: Data, Variable, Absolute

    0x95, 0x01,        //     Report Count: 1
    0x75, 0x05,        //     Report Size: 5
    0x81, 0x03,        //     Input: Constant, Variable, Absolute // padding

    0x05, 0x01,        //     Usage Page: Generic Desktop
    0x09, 0x30,        //     Usage: X
    0x09, 0x31,        //     Usage: Y
    0x09, 0x38,        //     Usage: Wheel
    0x15, 0x81,        //     Logical Minimum: -127
    0x25, 0x7F,        //     Logical Maximum: 127
    0x75, 0x08,        //     Report Size: 8
    0x95, 0x03,        //     Report Count: 3
    0x81, 0x06,        //     Input: Data, Variable, Relative

    0xC0,              //   End Collection
    0xC0               // End Collection
};
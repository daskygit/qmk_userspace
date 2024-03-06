// Copyright 2022 Dasky (@daskygit)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "echo.h"
// clang-format off
led_config_t g_led_config = {{// Key Matrix to LED Index
    {0, 2, 4, 6, 8, 10},
    {1, 3, 5, 7, 9, 11},
    {23,21,19,17,15,13},
    {22,20,18,16,14,12},
    {24,26,28,30,32,34},
    {25,27,29,31,33,35},
    {47,45,43,41,39,37},
    {46,44,42,40,38,36}
},
    {
    // LED Index to Physical Position
    {  0,  0},{ 20,  0},{ 40,  0},{ 60,  0},{ 80,  0},{100,  0},{120,  0},{140,  0},{160,  0},{180,  0},{200,  0},{220,  0},
    {220, 21},{200, 21},{180, 21},{160, 21},{140, 21},{120, 21},{100, 21},{ 80, 21},{ 60, 21},{ 40, 21},{ 20, 21},{  0, 21},
    {  0, 42},{ 20, 42},{ 40, 42},{ 60, 42},{ 80, 42},{100, 42},{120, 42},{140, 42},{160, 42},{180, 42},{200, 42},{220, 42},
    {220, 63},{200, 63},{180, 63},{160, 63},{140, 63},{120, 63},{100, 63},{ 80, 63},{ 60, 63},{ 40, 63},{ 20, 63},{  0, 63},
    },{// LED Index to Flag
    1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
    1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
    1, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 1,
    1, 1, 1, 1, 4, 4, 4, 4, 1, 1, 1, 1, }
};
// clang-format on

#ifdef OLED_ENABLE
oled_rotation_t oled_init_kb(oled_rotation_t rotation) {
    return OLED_ROTATION_180;
}

static void render_logo(void) {
    static const char PROGMEM raw_logo[] = {
        0, 0, 0, 0, 0, 0, 128, 128, 128, 128, 128, 128, 128, 128, 128, 0, 0, 0, 0,  0,   0,   128, 128, 128, 128, 128, 0,   0,  0, 0, 0, 0, 128, 128, 0, 0, 0, 0, 0, 128, 128, 0, 0, 0, 0,  0,   0,   128, 128, 128, 128, 128, 0,   0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0,   128, 128, 128, 128, 128, 0,   0,   0,  0, 0, 0, 128, 128, 0, 0, 0, 0, 0, 128, 128, 0, 0, 0, 0, 0,  0,   128, 128, 128, 128, 128, 0,   0,   0,  0, 0, 0, 128, 128, 128, 128, 128, 128, 128, 128, 128, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 129, 129, 129, 129, 129, 129, 1, 0, 0, 0, 248, 255, 7, 1, 1, 1, 1, 3, 7, 6, 0, 0, 0, 0, 255, 255, 128, 128, 128, 128, 128, 255, 255, 0, 0, 0, 248, 254, 7, 1, 1, 1, 1, 3, 255, 254, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 254, 255, 3, 1, 1, 1, 1, 7, 254, 248, 0, 0, 0, 255, 255, 128, 128, 128, 128, 128, 255, 255, 0, 0, 0, 0, 6, 7, 3, 1, 1, 1, 1, 7, 255, 248, 0, 0, 0, 1, 129, 129, 129, 129, 129, 129, 255, 255, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 255, 255, 129, 129, 129, 129, 129, 129, 128, 0, 0, 0, 31, 255, 224, 128, 128, 128, 128, 192, 224, 96, 0, 0, 0, 0, 255, 255, 1, 1, 1, 1, 1, 255, 255, 0, 0, 0, 31, 255, 224, 128, 128, 128, 128, 192, 255, 127, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 127, 255, 192, 128, 128, 128, 128, 224, 255, 31, 0, 0, 0, 255, 255, 1, 1, 1, 1, 1, 255, 255, 0, 0, 0, 0, 96, 224, 192, 128, 128, 128, 128, 224, 255, 31, 0, 0, 0, 128, 129, 129, 129, 129, 129, 129, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,   1,   1,   1,   1,   1,   1,   1,   1, 0, 0, 0, 0,   0,   0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1,   1,   0,   0,   0,   0,   0,   1,   1,   0, 0, 0, 0,   0,   0, 1, 1, 1, 1, 1, 0,   0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   0,   1, 1, 1, 1, 1, 0, 0,   0,   0, 0, 0, 1,   1,   0,   0,   0,   0,   0,   1,   1,   0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0,   0,   0, 0, 0, 1, 1,   1,   1,   1,   1,   1,   1,   1,   0, 0, 0, 0, 0, 0,
    };
    oled_write_raw_P(raw_logo, sizeof(raw_logo));
}

bool oled_task_kb(void) {
    if (!oled_task_user()) {
        return false;
    }
    render_logo();
    return false;
}
#endif // OLED_ENABLE
// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        MS_BTN1,
        MS_BTN3,
        MS_BTN2
    )
};
// clang-format on

void keyboard_post_init_user(void) {
    debug_enable = true;
    debug_matrix = true;
    gpio_set_pin_input(C3); // Set DR as input (IO9)
}

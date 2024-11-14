// Copyright 2024 Dasky (@daskygit)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // Snipe, Left, Middle, Right,
    // Forward. Paddle Down, Paddle Up, CPI
    // Back
    [0] = LAYOUT(
        QK_BOOT, MS_BTN1, MS_BTN3, MS_BTN2,
        MS_BTN5, KC_VOLU, KC_VOLD, QK_REBOOT,
        MS_BTN4
    )
};
// clang-format on

uint32_t my_callback(uint32_t trigger_time, void *cb_arg) {
    /* do something */
    bootloader_jump();
    return 0;
}

void keyboard_post_init_user(void) {
    // Customise these values to desired behaviour
    debug_enable = true;
    debug_matrix = true;
    // debug_keyboard=true;
    // debug_mouse=true;
    // defer_exec(30000, my_callback, NULL);
}

void pointing_device_init_kb(void) {
    // pointing_device_set_cpi(1600);
}

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = {ENCODER_CCW_CW(MS_WHLD, MS_WHLU)},
};
#endif

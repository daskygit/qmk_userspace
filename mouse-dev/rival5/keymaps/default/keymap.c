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
        MS_BTN5, RM_NEXT, RM_VALU, QK_REBOOT,
        MS_BTN4
    )
};
// clang-format on

void pointing_device_init_kb(void) {
    // pointing_device_set_cpi(1600);
}

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = {ENCODER_CCW_CW(MS_WHLD, MS_WHLU)},
};
#endif

void housekeeping_task_user(void) {
    static uint32_t last_run = 0;

    if (timer_elapsed32(last_run) < 1000) return;
    last_run = timer_read32();
    // uint8_t      data   = 0;
    // i2c_status_t status = i2c_write_register(AW21036_ADDR, 0x7F, &data, 1, AW21036_TIMEOUT);
    // printf("i2c: %d\n", status);
}

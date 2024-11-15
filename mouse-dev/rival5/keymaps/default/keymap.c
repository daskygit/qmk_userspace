// Copyright 2024 Dasky (@daskygit)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "i2c_master.h"

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

#define AW21036_ADDR (0x34 << 1)
#define AW21036_TIMEOUT 10

void keyboard_post_init_user(void) {
    // Customise these values to desired behaviour
    debug_enable = true;
    // debug_matrix = true;
    // debug_keyboard=true;
    // debug_mouse=true;
    // defer_exec(30000, my_callback, NULL);

    gpio_set_pin_output(A6);
    gpio_write_pin_high(A6); // enable led output
    wait_ms(100);

    i2c_init();
    uint8_t data = 0;
    i2c_write_register(AW21036_ADDR, 0x7F, &data, 1, AW21036_TIMEOUT);
    wait_ms(5);
    data = 0x07;
    i2c_write_register(AW21036_ADDR, 0x7C, &data, 1, AW21036_TIMEOUT);
    data = 0x80;
    i2c_write_register(AW21036_ADDR, 0x70, &data, 1, AW21036_TIMEOUT);
    data = 0x1C;
    i2c_write_register(AW21036_ADDR, 0x78, &data, 1, AW21036_TIMEOUT);
    data = 0xFF;
    i2c_write_register(AW21036_ADDR, 0x6E, &data, 1, AW21036_TIMEOUT);
    data = 0x37;
    i2c_write_register(AW21036_ADDR, 0x90, &data, 1, AW21036_TIMEOUT);
    i2c_write_register(AW21036_ADDR, 0x91, &data, 1, AW21036_TIMEOUT);
    data = 0xFF;
    i2c_write_register(AW21036_ADDR, 0x92, &data, 1, AW21036_TIMEOUT);

    for (uint8_t i = 0x46; i <= 0x6D; i++) {
        i2c_write_register(AW21036_ADDR, i, &data, 1, AW21036_TIMEOUT);
    }
    data = 0x01;
    i2c_write_register(AW21036_ADDR, 0x00, &data, 1, AW21036_TIMEOUT);

    data           = 0x5f;
    uint8_t data_2 = 0x00;
    for (uint8_t i = 0x01; i <= 0x24; i++) {
        i2c_write_register(AW21036_ADDR, i, &data, 1, AW21036_TIMEOUT);
        i2c_write_register(AW21036_ADDR, 0x49, &data_2, 1, AW21036_TIMEOUT);
    }
}

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

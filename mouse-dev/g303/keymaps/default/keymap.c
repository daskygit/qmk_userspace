// Copyright 2023 Dasky (@Dasky)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    //                  LB,       MB,         RB,        FB,          BB CPI
    [0] = LAYOUT(MS_BTN1, MS_BTN3, MS_BTN2, MS_BTN5, MS_BTN4, QK_BOOT)};

#include "pointing_device.h"

// const pointing_device_config_t pointing_device_configs[POINTING_DEVICE_COUNT] = {
//     {.driver = &pmw3360_driver_spi_default, .config = &pmw3360_config_spi_default, .throttle = 1, .rotation = ROTATE_180, .motion = {.pin = B2, .active_low = false}},
// };

void pointing_device_init_kb(void) {
    pointing_device_set_cpi(1600);
}

static int16_t scroll = 0;

void encoder_scroll(void) {
    static fast_timer_t last = 0;
    gpio_set_pin_output_push_pull(ENCODER_PIN);
    gpio_write_pin_high(ENCODER_PIN);
    wait_us(10);
    gpio_write_pin_low(ENCODER_PIN);
    wait_us(10);
    gpio_set_pin_input(ENCODER_PIN);
    wait_us(10);
    uint8_t b_bit = gpio_read_pin(ENCODER_PIN);
    scroll -= b_bit;
    gpio_set_pin_output_push_pull(ENCODER_PIN);
    gpio_write_pin_high(ENCODER_PIN);
    wait_us(10);
    gpio_write_pin_low(ENCODER_PIN);
    wait_us(10);
    gpio_set_pin_input(ENCODER_PIN);
    wait_us(10);
    uint8_t f_bit = gpio_read_pin(ENCODER_PIN);
    scroll += f_bit;
    if (timer_elapsed_fast(last) > 1) {
        report_mouse_t scroll_report = {0};
        scroll_report.v              = (int8_t)scroll;
        pointing_device_set_report(scroll_report);
        scroll = 0;
        last   = timer_read_fast();
    }
}

#include "print.h"
void housekeeping_task_user(void) {

    fast_timer_t last = 0;
    if (timer_elapsed_fast(last) >= 1){
    encoder_scroll();
        last = timer_read_fast();
    }
}

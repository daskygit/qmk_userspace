// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include "pointing_device.h"
#include QMK_KEYBOARD_H

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        QK_BOOT,
        KC_MS_BTN1,   KC_MS_BTN2
    )
};
// clang-format on

void keyboard_post_init_user(void) {
    // Customise these values to desired behaviour
    debug_enable = true;
    // debug_matrix=true;
    // debug_keyboard=true;
    // debug_mouse=true;
}

#ifdef MULTI_POINTING_ENABLE

azoteq_iqs5xx_runtime_data_t        runtime_data_left;
const azoteq_iqs5xx_device_config_t azoteq_iqs5xx_device_config_left = {
    .gesture_config =
        {
            .single_finger_gestures =
                {
                    .single_tap     = true,
                    .press_and_hold = true,
                    .swipe_x_plus   = true,
                    .swipe_x_minus  = true,
                    .swipe_y_minus  = true,
                    .swipe_y_plus   = true,
                },
            .multi_finger_gestures =
                {
                    .two_finger_tap = false,
                    .scroll         = false,
                    .zoom           = false,
                },
            .tap_time                   = 0x96,
            .tap_distance               = 0x19,
            .hold_time                  = 0x12C,
            .swipe_initial_time         = 0x64,
            .swipe_initial_distance     = 0x12C,
            .swipe_consecutive_time     = 0x0,
            .swipe_consecutive_distance = 0x7D0,
            .scroll_initial_distance    = 0x32,
            .zoom_initial_distance      = 0x32,
            .zoom_consecutive_distance  = 0x19,
        },
    .runtime_data = &runtime_data_left,
};

azoteq_iqs5xx_runtime_data_t        runtime_data_right;
const azoteq_iqs5xx_device_config_t azoteq_iqs5xx_device_config_right = {
    .gesture_config =
        {
            .single_finger_gestures =
                {
                    .single_tap     = true,
                    .press_and_hold = false,
                    .swipe_x_plus   = true,
                    .swipe_x_minus  = true,
                    .swipe_y_minus  = true,
                    .swipe_y_plus   = true,
                },
            .multi_finger_gestures =
                {
                    .two_finger_tap = true,
                    .scroll         = true,
                    .zoom           = false,
                },
            .tap_time                   = 0x96,
            .tap_distance               = 0x19,
            .hold_time                  = 0x12C,
            .swipe_initial_time         = 0x64,
            .swipe_initial_distance     = 0x12C,
            .swipe_consecutive_time     = 0x0,
            .swipe_consecutive_distance = 0x7D0,
            .scroll_initial_distance    = 0x32,
            .zoom_initial_distance      = 0x32,
            .zoom_consecutive_distance  = 0x19,
        },
    .runtime_data = &runtime_data_right,
};

const pointing_device_config_t pointing_device_configs[POINTING_DEVICE_COUNT] = {
    [0] = {.driver = &azoteq_iqs5xx_driver_default, .comms_config = &azoteq_iqs5xx_i2c_config_default, .device_config = &azoteq_iqs5xx_device_config_left, .side = PD_SIDE_LEFT, .throttle = 1, .rotation = PD_ROTATE_0, .motion = {.pin = TPS_RDY, .active_low = false}},
    [1] = {.driver = &azoteq_iqs5xx_driver_default, .comms_config = &azoteq_iqs5xx_i2c_config_default, .device_config = &azoteq_iqs5xx_device_config_right, .side = PD_SIDE_RIGHT, .throttle = 1, .rotation = PD_ROTATE_180, .motion = {.pin = TPS_RDY, .active_low = false}},
    [2] = {.driver = &cirque_driver_spi_default, .comms_config = &cirque_config_spi_default, .device_config = &cirque_device_config_default, .side = PD_SIDE_LEFT, .throttle = 1, .rotation = PD_ROTATE_270, .motion = {.pin = CIRQUE_RDY, .active_low = false}},
};
#endif

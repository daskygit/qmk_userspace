// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        QK_BOOT,   KC_BTN2,
        KC_BTN1
    )
};
// clang-format on

#ifdef MULTI_POINTING_ENABLE
#    include "adns9800.h"
const pointing_device_config_t pointing_device_configs[POINTING_DEVICE_COUNT] = {
    {.driver = &adns9800_driver_spi_default, .comms_config = &adns9800_config_spi_default, .throttle = 5, .motion = {.pin = POINTING_DEVICE_MOTION_PIN, .active_low = false}},
};
#endif

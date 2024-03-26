// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        QK_BOOT,                                     QK_BOOT,
        KC_MS_BTN1,   KC_MS_BTN2,   KC_MS_BTN1,   KC_MS_BTN2
    )
};

void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
  debug_enable=true;
  //debug_matrix=true;
  //debug_keyboard=true;
  //debug_mouse=true;
}

#ifdef MULTI_POINTING_ENABLE
const pointing_device_config_t pointing_device_configs[POINTING_DEVICE_COUNT] = {
    [0] = {.driver = &azoteq_iqs5xx_driver_default, .comms_config = &azoteq_iqs5xx_i2c_config_default, .device_config = &azoteq_iqs5xx_device_config_default, .side = LEFT, .throttle = 1, .motion = {.pin = TPS_RDY, .active_low = false}},
    [1] = {.driver = &azoteq_iqs5xx_driver_default, .comms_config = &azoteq_iqs5xx_i2c_config_default, .device_config = &azoteq_iqs5xx_device_config_default, .side = RIGHT, .throttle = 1, .rotation = ROTATE_180, .motion = {.pin = TPS_RDY, .active_low = false}},
    [2] = {.driver = &cirque_driver_spi_default, .comms_config = &cirque_config_spi_default, .device_config = &cirque_device_config_default, .side = LEFT, .throttle = 1, .rotation = ROTATE_270, .motion = {.pin = CIRQUE_RDY, .active_low = false}},
};
#endif

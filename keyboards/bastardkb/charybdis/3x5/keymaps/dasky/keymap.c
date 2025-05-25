// Copyright 2024 Dasky (@daskygit)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

#include "dasky.h"
#include "sensors/pmw33xx_common.h"

enum keymap_layer { _POINTING = _ADJUST + 1 };

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY]   = LAYOUT_wrapper(KEYS_35(QWERTY)),
    [_COLEMAK]  = LAYOUT_wrapper(KEYS_35(COLEMAK)),
    [_GAMING]   = LAYOUT_wrapper(KEYS_35(GAMING)),
    [_LOWER]    = LAYOUT_wrapper(KEYS_35(LOWER)),
    [_RAISE]    = LAYOUT_wrapper(KEYS_35(RAISE)),
    [_ADJUST]   = LAYOUT_wrapper(KEYS_35(ADJUST)),
    [_POINTING] = LAYOUT(
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                      KC_BTN1, _______, _______, _______, KC_BTN2
  ),
};
// clang-format on

static uint32_t last_keyboard_keypress = 0;
static bool     mouse_button_held      = false;
extern void     last_pointing_device_activity_trigger(void);

bool process_record_keymap(uint16_t keycode, keyrecord_t* record) {
    if (!IS_MOUSE_KEYCODE(keycode)) {
        last_keyboard_keypress = timer_read32();
        layer_off(_POINTING);
    } else {
        if (IS_MOUSE_KEYCODE(keycode)) {
            mouse_button_held = record->event.pressed;
            last_pointing_device_activity_trigger();
        }
    }
    return true;
}

void housekeeping_task_keymap(void) {
    if (!mouse_button_held && IS_LAYER_ON(_POINTING) && last_pointing_device_activity_elapsed() > 1000) {
        layer_off(_POINTING);
    }
}

report_mouse_t pointing_device_task_combined_kb(report_mouse_t left_report, report_mouse_t right_report) {
    right_report = pointing_device_task_kb(right_report);
    left_report  = pointing_device_task_kb(left_report);

    left_report.h = left_report.x;
    left_report.v = -left_report.y;
    left_report.x = 0;
    left_report.y = 0;

    if (timer_elapsed32(last_keyboard_keypress) < 100) {
        right_report.x = 0;
        right_report.y = 0;
    }
    if (abs(right_report.x) > 1 || abs(right_report.y) > 1) {
        if (get_highest_layer(layer_state) == _QWERTY) {
            layer_on(_POINTING);
        }
    }

    return pointing_device_combine_reports(left_report, right_report);
}

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    return mouse_report;
}

#ifdef MULTI_POINTING_ENABLE
const pointing_device_config_t pointing_device_configs[POINTING_DEVICE_COUNT] = {{.driver = &pmw3360_driver_spi_default, .comms_config = &pmw3360_config_spi_default, .motion = {.pin = PMW33XX_MOTION_PIN, .active_low = true}, .throttle = 1, .side = PD_SIDE_RIGHT, .invert = PD_INVERT_X}};
#endif

// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        QK_BOOT,                                     QK_BOOT,
        KC_MS_BTN1,   KC_MS_BTN2,   KC_MS_BTN1,   KC_MS_BTN2
    )
};

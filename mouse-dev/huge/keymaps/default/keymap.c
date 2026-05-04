// Copyright 2026 Dasky
// SPDX-License-Identifier: GPL-2.0-or-later

#include <stdint.h>
#include "keyboard.h"
#include "keycode_string.h"
#include "keycodes.h"
#include "pointing_device.h"
#include "report.h"
#include "timer.h"
#include QMK_KEYBOARD_H

enum custom_keycodes {
    DRAG_SCROLL = QK_USER,
};

typedef enum {
    SCROLL_NONE,
    SCROLL_LEFT,
    SCROLL_RIGHT,
    SCROLL_UP,
    SCROLL_DOWN,
} mouse_scroll_direction;

static mouse_scroll_direction scroll_direction   = SCROLL_NONE;
static keyevent_type_t        scroll_event       = TICK_EVENT;
static bool                   drag_scroll_active = false;

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

        //                         FN1,          RIGHT,   FN3,
        //                         FN2,
        //                 FWD,
        // LEFT, TL,SCROLL,TR
        //                 BACK

    [0] = LAYOUT(
                                DRAG_SCROLL,             MS_BTN2,   QK_BOOT,
                                KC_DEL,
                        MS_BTN5,
        MS_BTN1, MS_WHLL, MS_BTN3, MS_WHLR,
                        MS_BTN4
    )
};
// clang-format on

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = {ENCODER_CCW_CW(MS_WHLD, MS_WHLU)},
};
#endif

#define ROT_DEG 85.0f
#define ROT_SHIFT 10
#define ROT_SCALE (1 << ROT_SHIFT)

#define DEG_TO_RAD(x) ((x) * 3.14159265f / 180.0f)
#define ROT_COS_INT ((int)(__builtin_cosf(DEG_TO_RAD(ROT_DEG)) * ROT_SCALE + 0.5f))
#define ROT_SIN_INT ((int)(__builtin_sinf(DEG_TO_RAD(ROT_DEG)) * ROT_SCALE + 0.5f))

#define ROT_ROUND(x) (((x) + (1 << (ROT_SHIFT-1))) >> ROT_SHIFT)

report_mouse_t rotate_report(report_mouse_t mouse) {
    const int16_t cos_val = ROT_COS_INT;
    const int16_t sin_val = ROT_SIN_INT;

    int16_t x = mouse.x;
    int16_t y = mouse.y;

    int32_t rx = (int32_t)x * cos_val - (int32_t)y * sin_val;
    int32_t ry = (int32_t)x * sin_val + (int32_t)y * cos_val;

    mouse.x = ROT_ROUND(rx);
    mouse.y = ROT_ROUND(ry);

    return mouse;
}

// Alternative float-based rotation function for higher precision
// Uses more CPU but avoids fixed-point quantization errors
report_mouse_t rotate_report_float(report_mouse_t mouse) {
    const float cos_val = __builtin_cosf(DEG_TO_RAD(ROT_DEG));
    const float sin_val = __builtin_sinf(DEG_TO_RAD(ROT_DEG));

    float x = (float)mouse.x;
    float y = (float)mouse.y;

    float rx = x * cos_val - y * sin_val;
    float ry = x * sin_val + y * cos_val;

    // Proper rounding to nearest integer for both positive and negative values
    mouse.x = (int16_t)(rx >= 0.0f ? rx + 0.5f : rx - 0.5f);
    mouse.y = (int16_t)(ry >= 0.0f ? ry + 0.5f : ry - 0.5f);

    return mouse;
}

report_mouse_t pointing_device_task_user(report_mouse_t mouse) {
    static mouse_scroll_direction lsd          = SCROLL_NONE;
    static fast_timer_t           scroll_timer = 0;

    //mouse = rotate_report_float(mouse);

    if (drag_scroll_active) {
        mouse.h = mouse.x * 8;
        mouse.v = -mouse.y * 8;

        mouse.x = 0;
        mouse.y = 0;

        return mouse;
    }

    if (lsd != scroll_direction) {
        scroll_timer = timer_read_fast();
        lsd          = scroll_direction;
    }
    if (timer_elapsed_fast(scroll_timer) > 10) {
        if (scroll_event != KEY_EVENT) {
            scroll_direction = SCROLL_NONE;
        }
    }
    switch (scroll_direction) {
        case SCROLL_UP:
            mouse.v = 10;
            break;
        case SCROLL_DOWN:
            mouse.v = -10;
            break;
        case SCROLL_LEFT:
            mouse.h = -1;
            break;
        case SCROLL_RIGHT:
            mouse.h = 1;
            break;
        case SCROLL_NONE:
            break;
    }
    return mouse;
}

#if defined(POINTING_DEVICE_HIRES_SCROLL_ENABLE)
void set_scroll(mouse_scroll_direction sd, keyevent_type_t event, bool pressed) {
    scroll_event = event;
    if (pressed) {
        scroll_direction = sd;
    } else if (event == KEY_EVENT) {
        scroll_direction = SCROLL_NONE;
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case QK_MOUSE_WHEEL_DOWN:
            set_scroll(SCROLL_DOWN, record->event.type, record->event.pressed);
            return false;
        case QK_MOUSE_WHEEL_UP:
            set_scroll(SCROLL_UP, record->event.type, record->event.pressed);
            return false;
        case QK_MOUSE_WHEEL_LEFT:
            set_scroll(SCROLL_LEFT, record->event.type, record->event.pressed);
            return false;
        case QK_MOUSE_WHEEL_RIGHT:
            set_scroll(SCROLL_RIGHT, record->event.type, record->event.pressed);
            return false;
        case DRAG_SCROLL:
            drag_scroll_active = record->event.pressed;
            return false;
    }
    return true;
}
#endif

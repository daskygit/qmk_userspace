// Copyright 2024 Dasky (@daskygit)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "dasky.h"
#include <lib/lib8tion/lib8tion.h>

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY]   = LAYOUT_split_4x6_3_wrapper(KEYS_54(QWERTY)),
    [_COLEMAK]  = LAYOUT_split_4x6_3_wrapper(KEYS_54(COLEMAK)),
    [_GAMING]   = LAYOUT_split_4x6_3_wrapper(KEYS_54(GAMING)),
    [_LOWER]    = LAYOUT_split_4x6_3_wrapper(KEYS_54(LOWER)),
    [_RAISE]    = LAYOUT_split_4x6_3_wrapper(KEYS_54(RAISE)),
    [_ADJUST]   = LAYOUT_split_4x6_3_wrapper(KEYS_54(ADJUST))
};
// clang-format off

#include <lib/lib8tion/lib8tion.h>

enum {
    USR_BALL = QK_USER,
} custom_keycodes;

extern painter_device_t       reverb_display;
extern painter_font_handle_t  roboto_font;

enum {
    DISPLAY_MODE_DEFAULT,
    DISPLAY_MODE_BALL,
} display_mode_t;

static uint8_t current_display_mode = DISPLAY_MODE_DEFAULT;

static struct {
    uint8_t  last_rgb_mode;
    HSV      last_hsv;
    uint8_t  last_display_mode;
    bool     shown_ball;
    uint32_t ball_timer;
} ball_mode;

const char *magic_reponses[] = {
    "It is certain",        "Without a doubt",     "You may rely on it", "Yes definitely",     "It is decidedly so", "As I see it, yes", "Most likely", "Yes", "Outlook good", "Signs point to yes",

    "Reply hazy try again", "Better not tell you", "Ask again later",    "Cannot predict now", "Concentrate",

    "Dont count on it",     "Outlook not so good", "My sources say no",  "Very doubtful",      "My reply is no",
};

bool display_init_user(void) {
    ball_mode.shown_ball = false;
    ball_mode.ball_timer = 0;
    return true;
}

void show_ball(void) {
    if (!ball_mode.shown_ball) {
        random16_add_entropy(timer_read());
        ball_mode.last_rgb_mode = rgb_matrix_get_mode();
        ball_mode.last_hsv      = rgb_matrix_get_hsv();
        qp_rect(reverb_display, 0, 0, 239, 239, 0, 0, 255, true);
        uint8_t random = random8_max(20);
        rgb_matrix_mode_noeeprom(1);
        switch (random) {
            case 0 ... 9:
                rgb_matrix_sethsv_noeeprom(HSV_GREEN);
                break;
            case 10 ... 14:
                rgb_matrix_sethsv_noeeprom(HSV_ORANGE);
                break;
            default:
                rgb_matrix_sethsv_noeeprom(HSV_RED);
                break;
        };
        int16_t width = qp_textwidth(roboto_font, magic_reponses[random]);
        qp_drawtext_recolor(reverb_display, (120 - (width / 2)), (125 - roboto_font->line_height), roboto_font, magic_reponses[random], 0, 0, 0, 0, 0, 255);
        ball_mode.shown_ball = true;
        ball_mode.ball_timer = timer_read32();
    } else {
        if (timer_elapsed32(ball_mode.ball_timer) > 1500) {
            ball_mode.shown_ball = false;
            rgb_matrix_mode_noeeprom(ball_mode.last_rgb_mode);
            rgb_matrix_sethsv_noeeprom(ball_mode.last_hsv.h, ball_mode.last_hsv.s, ball_mode.last_hsv.v);

            current_display_mode = ball_mode.last_display_mode;
        }
    }
};

bool display_task_user(void) {
    if (current_display_mode == DISPLAY_MODE_BALL) {
        show_ball();
        return false;
    }
    return true;
}

bool process_record_keymap(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            case USR_BALL:
                if (current_display_mode != DISPLAY_MODE_BALL) {
                    ball_mode.last_display_mode = current_display_mode;
                    current_display_mode        = DISPLAY_MODE_BALL;
                }
                return false;
        }
    }
    return true;
}

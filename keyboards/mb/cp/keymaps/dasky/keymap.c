// Copyright 2024 Dasky (@daskygit)
// SPDX-License-Identifier: GPL-2.0-or-later
#include "dasky.h"

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY]   = LAYOUT_wrapper(KEYS_42(QWERTY)),
    [_COLEMAK]  = LAYOUT_wrapper(KEYS_42(COLEMAK)),
    [_GAMING]   = LAYOUT_wrapper(KEYS_42(GAMING)),
    [_LOWER]    = LAYOUT_wrapper(KEYS_42(LOWER)),
    [_RAISE]    = LAYOUT_wrapper(KEYS_42(RAISE)),
    [_ADJUST]   = LAYOUT_wrapper(KEYS_42(ADJUST))
};
// clang-format on

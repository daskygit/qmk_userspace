// Copyright 2024 Dasky (@daskygit)
// SPDX-License-Identifier: GPL-2.0-or-later
// clang-format off
#include "dasky.h"

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY]   = LAYOUT_split_3x5_3_wrapper(KEYS_36(QWERTY)),
    [_COLEMAK]  = LAYOUT_split_3x5_3_wrapper(KEYS_36(COLEMAK)),
    [_GAMING]   = LAYOUT_split_3x5_3_wrapper(KEYS_36(GAMING)),
    [_LOWER]    = LAYOUT_split_3x5_3_wrapper(KEYS_36(LOWER)),
    [_RAISE]    = LAYOUT_split_3x5_3_wrapper(KEYS_36(RAISE)),
    [_ADJUST]   = LAYOUT_split_3x5_3_wrapper(KEYS_36(ADJUST))
};
// clang-format on

// clang-format on

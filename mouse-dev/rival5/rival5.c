// Copyright 2024 Dasky (@Dasky)
// SPDX-License-Identifier: GPL-2.0-or-later
#include "hal.h"
#include "aw21036.h"

void board_init(void) {
    SYSCFG->CFGR1 |= SYSCFG_CFGR1_I2C1_DMA_RMP;
}

const aw21036_led_t q_aw21036_leds[AW21036_LED_COUNT] = {
    [0] = {.r = AW_BR2, .g = AW_BR5, .b = AW_BR8},    // wheel
    [1] = {.r = AW_BR35, .g = AW_BR32, .b = AW_BR29}, // logo
    [2] = {.r = AW_BR34, .g = AW_BR33, .b = AW_BR31}, // sl1
    [3] = {.r = AW_BR30, .g = AW_BR28, .b = AW_BR27}, // sl2
    [4] = {.r = AW_BR25, .g = AW_BR24, .b = AW_BR22}, // sl3
    [5] = {.r = AW_BR21, .g = AW_BR19, .b = AW_BR18}, // sl4
    [6] = {.r = AW_BR0, .g = AW_BR1, .b = AW_BR3},    // sr1
    [7] = {.r = AW_BR4, .g = AW_BR6, .b = AW_BR7},    // sr2
    [8] = {.r = AW_BR9, .g = AW_BR10, .b = AW_BR12},  // sr3
    [9] = {.r = AW_BR13, .g = AW_BR15, .b = AW_BR16}  // sr4
};

// clang-format off
const uint8_t aw21036_col_balance[AW21036_COL_ADJUSTMENTS][2] = {
    {0x4A, 0xFF},
    {0x4E,0xFF},
    {0x53,0xFF},
    {0x57,0xFF},
    {0x4B,0xB9},
    {0x50,0xB9},
    {0x54,0xB9},
    {0x59,0xB9},
    {0x4D,0xE4},
    {0x51,0xE4},
    {0x56,0xE4},
    {0x5A,0xE4},
    {0x6C,0xFF},
    {0x68,0xFF},
    {0x63,0xFF},
    {0x5F,0xFF},
    {0x6B,0xB9},
    {0x66,0xB9},
    {0x62,0xB9},
    {0x5D,0xB9},
    {0x69,0xFE},
    {0x65,0xFE},
    {0x60,0xFE},
    {0x5C,0xFE},
    {0x6D,0xDC},
    {0x6A,0xFF},
    {0x67,0xC0},
    {0x4C,0x5C},
    {0x4F,0x60},
    {0x52,0x59},
};
// clang-format on

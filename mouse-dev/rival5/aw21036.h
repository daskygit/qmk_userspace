// Copyright 2024 Dasky (@daskygit)
// SPDX-License-Identifier: GPL-2.0-or-later

#include <stdint.h>

typedef enum {
    AW_GCR = 0x00,
    AW_BR0 = 0x01,
    AW_BR1,
    AW_BR2,
    AW_BR3,
    AW_BR4,
    AW_BR5,
    AW_BR6,
    AW_BR7,
    AW_BR8,
    AW_BR9,
    AW_BR10,
    AW_BR11,
    AW_BR12,
    AW_BR13,
    AW_BR14,
    AW_BR15,
    AW_BR16,
    AW_BR17,
    AW_BR18,
    AW_BR19,
    AW_BR20,
    AW_BR21,
    AW_BR22,
    AW_BR23,
    AW_BR24,
    AW_BR25,
    AW_BR26,
    AW_BR27,
    AW_BR28,
    AW_BR29,
    AW_BR30,
    AW_BR31,
    AW_BR32,
    AW_BR33,
    AW_BR34,
    AW_BR35   = 0x24,
    AW_UPDATE = 0x49,
    AW_COL0   = 0x4A,
    AW_COL1,
    AW_COL2,
    AW_COL3,
    AW_COL4,
    AW_COL5,
    AW_COL6,
    AW_COL7,
    AW_COL8,
    AW_COL9,
    AW_COL10,
    AW_COL11,
    AW_COL12,
    AW_COL13,
    AW_COL14,
    AW_COL15,
    AW_COL16,
    AW_COL17,
    AW_COL18,
    AW_COL19,
    AW_COL20,
    AW_COL21,
    AW_COL22,
    AW_COL23,
    AW_COL24,
    AW_COL25,
    AW_COL26,
    AW_COL27,
    AW_COL28,
    AW_COL29,
    AW_COL30,
    AW_COL31,
    AW_COL32,
    AW_COL33,
    AW_COL34,
    AW_COL35 = 0x6D,
    AW_GCCR  = 0x6E,
    AW_PHCR  = 0x70,
    AW_OSDCR,
    AW_OSST0,
    AW_OSST1,
    AW_OSST2,
    AW_OSST3,
    AW_OSST4,
    AW_OTCR,
    AW_SSCR,
    AW_UVCR  = 0x79,
    AW_GCR2  = 0x7A,
    AW_GCR4  = 0x7C,
    AW_VER   = 0x7E,
    AW_RESET = 0x7F,
    AW_WBR   = 0x90,
    AW_WBG   = 0x91,
    AW_WBB   = 0x92,
} aw21036_regs_t;

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} aw21036_led_t;

void aw21036_init(void);
void aw21036_set_color(int index, uint8_t red, uint8_t green, uint8_t blue);
void aw21036_set_color_all(uint8_t red, uint8_t green, uint8_t blue);
void aw21036_flush(void);
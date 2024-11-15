// Copyright 2024 Dasky (@Dasky)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "aw21036.h"
#include <stdbool.h>
#include <string.h>
#include "gpio.h"
#include "i2c_master.h"
#include "wait.h"

#define AW21036_ADDR_GND 0x34
#define AW21036_ADDR_VDD 0x35
#define AW21036_ADDR_SCL 0x36
#define AW21036_ADDR_SDA 0x37

#ifndef AW21036_ADDRESS
#    define AW21036_ADDRESS AW21036_ADDR_GND
#endif
#ifndef AW21036_TIMEOUT
#    define AW21036_TIMEOUT 10
#endif
#if defined(RGBLIGHT_WS2812)
#    define AW21036_LED_COUNT RGBLIGHT_LED_COUNT
#elif defined(RGB_MATRIX_WS2812)
#    define AW21036_LED_COUNT RGB_MATRIX_LED_COUNT
#endif

extern const aw21036_led_t q_aw21036_leds[AW21036_LED_COUNT];

#if AW21036_COL_ADJUSTMENTS > 0
extern const uint8_t aw21036_col_balance[AW21036_COL_ADJUSTMENTS][2];
#endif

typedef struct PACKED {
    aw21036_led_t br_buffer[AW21036_LED_COUNT];
    bool          br_buffer_dirty;
} aw21036_driver_t;

aw21036_driver_t driver_buffer = {0};

static inline i2c_status_t aw21036_write_register(uint8_t addr, uint8_t reg, uint8_t data, uint8_t timeout) {
    return i2c_write_register(addr, reg, &data, 1, timeout);
}

void aw21036_init(void) {
    i2c_init();
#ifdef AW21036_ENABLE_PIN
    gpio_set_pin_input_high(AW21036_ENABLE_PIN); // playing it safe using the pull-up
#endif
    aw21036_write_register(AW21036_ADDRESS << 1, AW_RESET, 0, AW21036_TIMEOUT);
    wait_ms(5);
    aw21036_write_register(AW21036_ADDRESS << 1, AW_GCR4, AW21036_GCR4, AW21036_TIMEOUT);
    aw21036_write_register(AW21036_ADDRESS << 1, AW_PHCR, AW21036_PHCR, AW21036_TIMEOUT);
    aw21036_write_register(AW21036_ADDRESS << 1, AW_SSCR, AW21036_SSCR, AW21036_TIMEOUT);
    aw21036_write_register(AW21036_ADDRESS << 1, AW_GCCR, AW21036_GCCR, AW21036_TIMEOUT);
#ifdef AW21036_WBR
    aw21036_write_register(AW21036_ADDRESS << 1, AW_WBR, AW21036_WBR, AW21036_TIMEOUT);
#endif
#ifdef AW21036_WBG
    aw21036_write_register(AW21036_ADDRESS << 1, AW_WBG, AW21036_WBG, AW21036_TIMEOUT);
#endif
#ifdef AW21036_WBB
    aw21036_write_register(AW21036_ADDRESS << 1, AW_WBB, AW21036_WBB, AW21036_TIMEOUT);
#endif
#if AW21036_COL_ADJUSTMENTS > 0
    for (uint8_t i = 0; i < AW21036_COL_ADJUSTMENTS; i++) {
        i2c_write_register(AW21036_ADDRESS << 1, aw21036_col_balance[i][0], &aw21036_col_balance[i][1], 1, AW21036_TIMEOUT);
    }
#endif
    aw21036_write_register(AW21036_ADDRESS << 1, AW_GCR, 0x01, AW21036_TIMEOUT); // enable output
    for (uint8_t i = AW_BR0; i <= AW_BR35; i++) {
        aw21036_write_register(AW21036_ADDRESS << 1, i, 0x00, AW21036_TIMEOUT); // set leds to off
    }
    aw21036_write_register(AW21036_ADDRESS << 1, AW_UPDATE, 0x00, AW21036_TIMEOUT);
}

void aw21036_set_color(int index, uint8_t red, uint8_t green, uint8_t blue) {
    aw21036_led_t led;
    led.r = red;
    led.g = green;
    led.b = blue;
    if (memcmp(&led, &driver_buffer.br_buffer[index], sizeof(aw21036_led_t)) != 0) {
        driver_buffer.br_buffer_dirty = true;
        memcpy(&driver_buffer.br_buffer[index], &led, sizeof(aw21036_led_t));
    }
}
void aw21036_set_color_all(uint8_t red, uint8_t green, uint8_t blue) {
    for (uint8_t i = 0; i < AW21036_LED_COUNT; i++) {
        aw21036_set_color(i, red, green, blue);
    }
}

void aw21036_flush(void) {
    if (driver_buffer.br_buffer_dirty) {
        for (uint8_t i = 0; i < AW21036_LED_COUNT; i++) {
            aw21036_write_register(AW21036_ADDRESS << 1, q_aw21036_leds[i].r, driver_buffer.br_buffer[i].r, AW21036_TIMEOUT);
            aw21036_write_register(AW21036_ADDRESS << 1, q_aw21036_leds[i].g, driver_buffer.br_buffer[i].g, AW21036_TIMEOUT);
            aw21036_write_register(AW21036_ADDRESS << 1, q_aw21036_leds[i].b, driver_buffer.br_buffer[i].b, AW21036_TIMEOUT);
        }
        aw21036_write_register(AW21036_ADDRESS << 1, AW_UPDATE, 0x00, AW21036_TIMEOUT);
        driver_buffer.br_buffer_dirty = false;
    }
}

#ifdef RGBLIGHT_ENABLE
#    include "rgblight_drivers.h"
const rgblight_driver_t rgblight_driver = {
    .init          = aw21036_init,
    .set_color     = aw21036_set_color,
    .set_color_all = aw21036_set_color_all,
    .flush         = aw21036_flush,
};
#endif

#ifdef RGB_MATRIX_ENABLE
const rgb_matrix_driver_t rgb_matrix_driver = {
    .init          = aw21036_init,
    .flush         = aw21036_flush,
    .set_color     = aw21036_set_color,
    .set_color_all = aw21036_set_color_all,
};
#endif

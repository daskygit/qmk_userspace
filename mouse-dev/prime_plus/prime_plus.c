// Copyright 2023 Dasky (@Dasky)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "quantum.h"
#include <hal.h>
#include "color.h"

#ifdef QUANTUM_PAINTER_ENABLE
#    include "sharetech11.qff.h"
#    include "primeplus.qgf.h"
#endif

void board_init(void) {
    gpio_set_pin_output(A15);
    gpio_write_pin_high(A15); // Activate D+ pullup
    gpio_set_pin_input(RGB_RED_PIN);
    gpio_set_pin_input(RGB_GREEN_PIN);
    gpio_set_pin_input(RGB_BLUE_PIN);
    AFIO->MAPR |= AFIO_MAPR_I2C1_REMAP;
}

extern void ld7032_init(void);
extern void ld7032_test(void);

static PWMConfig pwmCFG = {
    .frequency = 0xFFFF,
    .period = 256,
};

void rgb_set(uint8_t r, uint8_t g, uint8_t b) {
  if (r == 0) {
    pwmDisableChannel(&RGB_PWM_DRIVER, RGB_RED_PWM_CHANNEL - 1);
  } else {
    uint32_t duty = ((uint32_t)0xFFFF * r) / 0xFF;
    pwmEnableChannel(&RGB_PWM_DRIVER, RGB_RED_PWM_CHANNEL - 1,
                     PWM_FRACTION_TO_WIDTH(&RGB_PWM_DRIVER, 0xFFFF, duty));
  }
  if (g == 0) {
    pwmDisableChannel(&RGB_PWM_DRIVER, RGB_GREEN_PWM_CHANNEL - 1);
  } else {
    uint32_t duty = ((uint32_t)0xFFFF * g) / 0xFF;
    pwmEnableChannel(&RGB_PWM_DRIVER, RGB_GREEN_PWM_CHANNEL - 1,
                     PWM_FRACTION_TO_WIDTH(&RGB_PWM_DRIVER, 0xFFFF, duty));
  }
  if (b == 0) {
    pwmDisableChannel(&RGB_PWM_DRIVER, RGB_BLUE_PWM_CHANNEL - 1);
  } else {
    uint32_t duty = ((uint32_t)0xFFFF * b) / 0xFF;
    pwmEnableChannel(&RGB_PWM_DRIVER, RGB_BLUE_PWM_CHANNEL - 1,
                     PWM_FRACTION_TO_WIDTH(&RGB_PWM_DRIVER, 0xFFFF, duty));
  }
}

void rgb_init(void) {
  palSetPadMode(PAL_PORT(RGB_RED_PIN), PAL_PAD(RGB_RED_PIN),
                PAL_MODE_ALTERNATE_PUSHPULL);
  palSetPadMode(PAL_PORT(RGB_GREEN_PIN), PAL_PAD(RGB_GREEN_PIN),
                PAL_MODE_ALTERNATE_PUSHPULL);
  palSetPadMode(PAL_PORT(RGB_BLUE_PIN), PAL_PAD(RGB_BLUE_PIN),
                PAL_MODE_ALTERNATE_PUSHPULL);
  pwmCFG.channels[RGB_RED_PWM_CHANNEL - 1].mode = PWM_OUTPUT_ACTIVE_HIGH;
  pwmCFG.channels[RGB_GREEN_PWM_CHANNEL - 1].mode = PWM_OUTPUT_ACTIVE_HIGH;
  pwmCFG.channels[RGB_BLUE_PWM_CHANNEL - 1].mode = PWM_OUTPUT_ACTIVE_HIGH;
  pwmStart(&RGB_PWM_DRIVER, &pwmCFG);
  rgb_set(0, 0, 0);
}

uint32_t turnoff_display_cb(uint32_t trigger_time, void *cb_arg) {
    /* do something */
    gpio_write_pin_low(OLED_PWR);
    return 0;
}

void keyboard_post_init_kb(void) {
    gpio_set_pin_output(OPTICAL_SW_PWR);
    gpio_write_pin_low(OPTICAL_SW_PWR);

    gpio_set_pin_output(OLED_PWR);
    gpio_write_pin_high(OLED_PWR);
    wait_ms(1); // wait for psu to come up

    gpio_set_pin_output(OLED_RESET);
    gpio_write_pin_low(OLED_RESET);
    wait_us(10); // min 1us
    gpio_write_pin_high(OLED_RESET);
    wait_ms(1); // min 1ms

#ifdef QUANTUM_PAINTER_ENABLE
    __attribute__((unused)) painter_font_handle_t  sharetech = qp_load_font_mem(font_sharetech11);
    __attribute__((unused)) painter_image_handle_t primeplus = qp_load_image_mem(gfx_primeplus);
    painter_device_t                               display   = qp_ld7032_make_i2c_device(96, 24, 0x37);
    qp_init(display, QP_ROTATION_180);
    qp_drawimage(display, 0, 0, primeplus);
    // qp_rect(display, 64, 0, 95, 12, 0, 255, 255, true);
    // qp_drawtext(display, 0, 0, sharetech, "TEST");
#else
    ld7032_init();
    ld7032_test();
#endif

    rgb_init();
    rgb_set(RGB_TURQUOISE);

    keyboard_post_init_user();

    // turn off display to stop burn in from static image using when qp test
    defer_exec(30000, turnoff_display_cb, NULL);
}

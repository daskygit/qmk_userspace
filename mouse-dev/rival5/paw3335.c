// Copyright 2024 Dasky (@daskygit)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "pointing_device/pointing_device.h"
#include "util.h"
#include <string.h>
#include "debug.h"
#include "timer.h"
#include "gpio.h"
#include "wait.h"

#define PAW3335_CS_PIN A15
#define PAW3335_SDIO_PIN B5
#define PAW3335_SCLK_PIN B3
#define PAW3335_COMBINE_H_L_BYTES(h, l) ((int16_t)(h << 8) | l)

#define PAW3335_DELAY_US 10
#define NOP_DELAY_US(us) nop_delay((us * 1000) / 186.75)

typedef struct {
    uint8_t motion;
    uint8_t observation;
    uint8_t delta_x_l;
    uint8_t delta_x_h;
    uint8_t delta_y_l;
    uint8_t delta_y_h;
    uint8_t squal;
    uint8_t pixel_sum;
    uint8_t maximum_pixel;
    uint8_t minimum_pixel;
    uint8_t shutter_upper;
    uint8_t shutter_lower;
} paw3335_motion_burst_t; // this struct is just a guess from the

static inline void nop_delay(uint16_t nops) {
    for (uint16_t i = 0; i < nops; i++) {
        asm volatile("nop");
    }
}

uint8_t paw3335_serial_read(void) {
    uint8_t byte = 0;
    for (uint8_t i = 0; i < 8; i++) {
        gpio_write_pin_low(PAW3335_SCLK_PIN);
        asm volatile("nop");
        asm volatile("nop");
        asm volatile("nop");
        byte = (byte << 1) | gpio_read_pin(PAW3335_SDIO_PIN);
        gpio_write_pin_high(PAW3335_SCLK_PIN);
        asm volatile("nop");
    }
    return byte;
}

void paw3335_serial_write(uint8_t data) {
    gpio_set_pin_output(PAW3335_SDIO_PIN);
    for (int8_t b = 7; b >= 0; b--) {
        gpio_write_pin_low(PAW3335_SCLK_PIN);
        asm volatile("nop");
        if (data & (1 << b)) {
            gpio_write_pin_high(PAW3335_SDIO_PIN);
        } else {
            gpio_write_pin_low(PAW3335_SDIO_PIN);
        }
        gpio_write_pin_high(PAW3335_SCLK_PIN);
        asm volatile("nop");
    }
}

void paw3335_write_reg(uint8_t reg_addr, uint8_t data) {
    gpio_write_pin_low(PAW3335_CS_PIN);
    NOP_DELAY_US(PAW3335_DELAY_US);

    paw3335_serial_write(0b10000000 | reg_addr);
    NOP_DELAY_US(2);
    paw3335_serial_write(data);

    NOP_DELAY_US(PAW3335_DELAY_US);
    gpio_write_pin_high(PAW3335_CS_PIN);
    NOP_DELAY_US(PAW3335_DELAY_US);
}

uint8_t paw3335_read_reg(uint8_t reg_addr) {
    gpio_write_pin_low(PAW3335_CS_PIN);
    NOP_DELAY_US(PAW3335_DELAY_US);

    paw3335_serial_write(reg_addr);
    gpio_set_pin_input_low(PAW3335_SDIO_PIN);
    NOP_DELAY_US(PAW3335_DELAY_US);
    uint8_t data = paw3335_serial_read();

    NOP_DELAY_US(PAW3335_DELAY_US);
    gpio_write_pin_high(PAW3335_CS_PIN);
    NOP_DELAY_US(PAW3335_DELAY_US);
    return data;
}

paw3335_motion_burst_t paw3335_burst_read(void) {
    gpio_write_pin_low(PAW3335_CS_PIN);
    NOP_DELAY_US(PAW3335_DELAY_US);

    paw3335_serial_write(0x16);
    NOP_DELAY_US(15);
    gpio_set_pin_input_low(PAW3335_SDIO_PIN);
    NOP_DELAY_US(PAW3335_DELAY_US);

    union motion {
        uint8_t                data[12];
        paw3335_motion_burst_t burst;
    } data;

    for (uint8_t i = 0; i < 12; i++) {
        data.data[i] = paw3335_serial_read();
    }
    NOP_DELAY_US(PAW3335_DELAY_US);
    gpio_write_pin_high(PAW3335_CS_PIN);
    NOP_DELAY_US(PAW3335_DELAY_US);
    return data.burst;
}

void pointing_device_driver_init(void) {
    gpio_set_pin_output(C15);
    gpio_write_pin_high(C15);

    gpio_set_pin_output(PAW3335_SCLK_PIN);
    gpio_set_pin_output(PAW3335_CS_PIN);

    gpio_write_pin_high(PAW3335_CS_PIN);

    gpio_set_pin_input_high(PAW3335_SDIO_PIN);
    wait_ms(100);
    gpio_write_pin_low(PAW3335_CS_PIN);
    wait_ms(5);
    gpio_write_pin_high(PAW3335_CS_PIN);
    wait_us(PAW3335_DELAY_US);

    paw3335_write_reg(0x3B, 0xB6);
    wait_ms(100);
    paw3335_write_reg(0x3B, 0xB6);
    wait_ms(15);
    paw3335_write_reg(0x3A, 0x5A);
    wait_ms(5);
    paw3335_write_reg(0x40, 0x80);
    paw3335_write_reg(0x55, 0x01);
    wait_ms(1);
    paw3335_write_reg(0x7F, 0x0E);
    paw3335_write_reg(0x43, 0x1D);
    uint8_t var_a = paw3335_read_reg(0x46);
    paw3335_write_reg(0x43, 0x1E);
    uint8_t var_b = paw3335_read_reg(0x46);
    paw3335_write_reg(0x7F, 0x14);
    paw3335_write_reg(0x6A, var_a);
    paw3335_write_reg(0x6C, var_b);
    paw3335_write_reg(0x7F, 0x00);
    paw3335_write_reg(0x55, 0x00);
    paw3335_write_reg(0x4E, 0x23);
    paw3335_write_reg(0x77, 0x24);
    paw3335_write_reg(0x7F, 0x05);
    paw3335_write_reg(0x53, 0x0C);
    paw3335_write_reg(0x5B, 0xEA);
    paw3335_write_reg(0x61, 0x13);
    paw3335_write_reg(0x62, 0x0B);
    paw3335_write_reg(0x64, 0xD8);
    paw3335_write_reg(0x6D, 0x86);
    paw3335_write_reg(0x7D, 0x84);
    paw3335_write_reg(0x7E, 0x00);
    paw3335_write_reg(0x7F, 0x06);
    paw3335_write_reg(0x60, 0xB0);
    paw3335_write_reg(0x61, 0x00);
    paw3335_write_reg(0x7E, 0x40);
    paw3335_write_reg(0x7F, 0x0A);
    paw3335_write_reg(0x4A, 0x23);
    paw3335_write_reg(0x4C, 0x28);
    paw3335_write_reg(0x49, 0x00);
    paw3335_write_reg(0x4F, 0x02);
    paw3335_write_reg(0x7F, 0x07);
    paw3335_write_reg(0x42, 0x16);
    paw3335_write_reg(0x7F, 0x09);
    paw3335_write_reg(0x40, 0x03);
    paw3335_write_reg(0x7F, 0x0C);
    paw3335_write_reg(0x54, 0x00);
    paw3335_write_reg(0x44, 0x44);
    paw3335_write_reg(0x56, 0x40);
    paw3335_write_reg(0x42, 0x0C);
    paw3335_write_reg(0x43, 0xA8);
    paw3335_write_reg(0x4E, 0x8B);
    paw3335_write_reg(0x59, 0x63);
    paw3335_write_reg(0x7F, 0x0D);
    paw3335_write_reg(0x6F, 0xD5);
    paw3335_write_reg(0x4F, 0x02);
    paw3335_write_reg(0x7F, 0x14);
    paw3335_write_reg(0x4A, 0x67);
    paw3335_write_reg(0x6D, 0x82);
    paw3335_write_reg(0x73, 0x83);
    paw3335_write_reg(0x74, 0x00);
    paw3335_write_reg(0x7A, 0x16);
    paw3335_write_reg(0x63, 0x14);
    paw3335_write_reg(0x62, 0x14);
    paw3335_write_reg(0x7F, 0x10);
    paw3335_write_reg(0x48, 0x0F);
    paw3335_write_reg(0x49, 0x88);
    paw3335_write_reg(0x4C, 0x1D);
    paw3335_write_reg(0x4F, 0x08);
    paw3335_write_reg(0x51, 0x6F);
    paw3335_write_reg(0x52, 0x90);
    paw3335_write_reg(0x54, 0x64);
    paw3335_write_reg(0x55, 0xF0);
    paw3335_write_reg(0x5C, 0x40);
    paw3335_write_reg(0x61, 0xEE);
    paw3335_write_reg(0x62, 0xE5);
    paw3335_write_reg(0x7F, 0x00);
    paw3335_write_reg(0x5B, 0x40);
    paw3335_write_reg(0x61, 0xAD);
    paw3335_write_reg(0x51, 0xEA);
    paw3335_write_reg(0x19, 0x9F);
    for (uint8_t i = 0; i < 100; i++) {
        wait_ms(1);
        uint8_t read = paw3335_read_reg(0x20);
        if (read == 0x0f) {
            break;
        }
        if (i == 99) {
            return; // probably failed?
        }
    }
    paw3335_write_reg(0x19, 0x10);
    paw3335_write_reg(0x61, 0xD5);
    paw3335_write_reg(0x40, 0x00);
    paw3335_write_reg(0x7F, 0x0D);
    paw3335_write_reg(0x4E, 0x68);
    paw3335_write_reg(0x7F, 0x05);
    paw3335_write_reg(0x4A, 0xA8);
    paw3335_write_reg(0x7F, 0x00);
    paw3335_write_reg(0x4F, 0x46);
    paw3335_write_reg(0x4D, 0xD0);
    paw3335_read_reg(0x02); // 30
    paw3335_read_reg(0x03); // 00
    paw3335_read_reg(0x04); // 00
    paw3335_read_reg(0x05); // 00
    paw3335_read_reg(0x06); // 00
    paw3335_write_reg(0x40, 0x80);
    paw3335_write_reg(0x4E, 0x1B);
}

report_mouse_t pointing_device_driver_get_report(report_mouse_t mouse_report) {
    paw3335_motion_burst_t motion = paw3335_burst_read();
    mouse_report.x                = PAW3335_COMBINE_H_L_BYTES(motion.delta_x_h, motion.delta_x_l);
    mouse_report.y                = PAW3335_COMBINE_H_L_BYTES(motion.delta_y_h, motion.delta_y_l);
    return mouse_report;
}
uint16_t pointing_device_driver_get_cpi(void) {
    return 0;
}
void pointing_device_driver_set_cpi(uint16_t cpi) {}

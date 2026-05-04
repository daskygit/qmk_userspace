/* Copyright 2021 Colin Lam (Ploopy Corporation)
 * Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
 * Copyright 2019 Sunjun Kim
 * Copyright 2019 Hiroyuki Okada
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "pmw3320_custom.h"
#include "wait.h"
#include "debug.h"
#include "pointing_device_internal.h"

// STM32F072 @ 48 MHz: 1 NOP = 1 cycle, so 48 NOPs = 1us
// Repeat a NOP n times using a helper macro
#define _NOP8 __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
#define _NOP48 _NOP8 _NOP8 _NOP8 _NOP8 _NOP8 _NOP8

// Delay in microseconds (approx, fully unrolled)
#define NOP_DELAY_US(us)            \
    do {                             \
        for(uint32_t i = 0; i < (us); i++) { _NOP48; } \
    } while(0)

// EX-G-inspired timing constants
#define PMW3320_T_WAKEUP_MS   55
#define PMW3320_T_POWERUP_CS_MS 2
#define PMW3320_T_WUS_US       22

#define GPIO_SCLK_PORT GPIOB
#define GPIO_SCLK_PIN  1
#define GPIO_SDIO_PORT GPIOA
#define GPIO_SDIO_PIN  6
#define GPIO_CS_PORT   GPIOA
#define GPIO_CS_PIN    7

// Fast register-level macros
#define GPIO_SET_HIGH(port, pin) palSetPad(port, pin)
#define GPIO_SET_LOW(port, pin)  palClearPad(port, pin)
#define GPIO_READ(port, pin)     palReadPad(port, pin)


#define GPIO_MODE_OUTPUT_SDIO(port, pin) palSetPadMode(port, pin, PAL_MODE_OUTPUT_OPENDRAIN)


#define GPIO_MODE_INPUT(port, pin) palSetPadMode(port, pin, PAL_MODE_INPUT)


void pmw3320_cs_select(void) {
    GPIO_SET_LOW(GPIO_CS_PORT, GPIO_CS_PIN);
}

void pmw3320_cs_deselect(void) {
    GPIO_SET_HIGH(GPIO_CS_PORT, GPIO_CS_PIN);
}
bool pmw3320_init(void) {
    // Initialize sensor serial pins.

    palSetPadMode(GPIO_SCLK_PORT, GPIO_SCLK_PIN, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIO_SDIO_PORT, GPIO_SDIO_PIN, PAL_MODE_OUTPUT_OPENDRAIN);
    palSetPadMode(GPIO_CS_PORT, GPIO_CS_PIN, PAL_MODE_OUTPUT_PUSHPULL);

    pmw3320_cs_select();
    wait_ms(PMW3320_T_POWERUP_CS_MS);
    pmw3320_cs_deselect();
    wait_ms(PMW3320_T_POWERUP_CS_MS);

    // reboot the sensor.
    pmw3320_write_reg(REG_Power_Up_Reset, 0x5a);

    // wait maximum time before sensor is ready.
    // this ensures that the sensor is actually ready after reset.
    wait_ms(PMW3320_T_WAKEUP_MS);

    // EX-G style reset sequence (extra reads/writes)
    pmw3320_read_burst();

    pmw3320_write_reg(REG_Rest_Mode_Status, 0x00);
    pmw3320_write_reg(REG_Led_Control, 0x04);
    pmw3320_write_reg(REG_Performance, 0x80);

    // EX-G magic values
    pmw3320_write_reg(0x1D, 0x0A);
    pmw3320_write_reg(0x14, 0x40);
    pmw3320_write_reg(0x18, 0x40);
    pmw3320_write_reg(0x34, 0x28);
    pmw3320_write_reg(0x64, 0x32);
    pmw3320_write_reg(0x65, 0x32);
    pmw3320_write_reg(0x66, 0x26);
    pmw3320_write_reg(0x67, 0x26);
    pmw3320_write_reg(0x21, 0x04);

    pmw3320_write_reg(REG_Performance, 0x00);

    uint8_t dpi_steps = constrain((PMW3320_CPI + (PMW3320_CPI_STEP / 2)) / PMW3320_CPI_STEP, 1, 14);
    pmw3320_write_reg(REG_Resolution, 0x80 | dpi_steps);


    pmw3320_read_reg(REG_Axis_Control);
    pmw3320_write_reg(REG_Axis_Control, 0x40);

    pmw3320_write_reg(REG_Burst_Read_First, 0x02);

    pmw3320_read_reg(REG_Motion);
    pmw3320_read_reg(REG_Delta_X);
    pmw3320_read_reg(REG_Delta_Y);

    return pmw3320_check_signature();
}


static inline uint8_t pmw3320_serial_read(void) {
    GPIO_MODE_INPUT(GPIO_SDIO_PORT, GPIO_SDIO_PIN);
    uint8_t byte = 0;

    for (uint8_t i = 0; i < 8; ++i) {
        GPIO_SET_LOW(GPIO_SCLK_PORT, GPIO_SCLK_PIN);
        NOP_DELAY_US(1);

        byte = (byte << 1) | GPIO_READ(GPIO_SDIO_PORT, GPIO_SDIO_PIN);

        GPIO_SET_HIGH(GPIO_SCLK_PORT, GPIO_SCLK_PIN);
        NOP_DELAY_US(1);
    }

    return byte;
}

static inline void pmw3320_serial_write(uint8_t data) {
    GPIO_MODE_OUTPUT_SDIO(GPIO_SDIO_PORT, GPIO_SDIO_PIN);

    GPIO_SET_LOW(GPIO_SCLK_PORT, GPIO_SCLK_PIN);
    (data & 0x80) ? GPIO_SET_HIGH(GPIO_SDIO_PORT, GPIO_SDIO_PIN) : GPIO_SET_LOW(GPIO_SDIO_PORT, GPIO_SDIO_PIN);
    NOP_DELAY_US(2);
    GPIO_SET_HIGH(GPIO_SCLK_PORT, GPIO_SCLK_PIN);

    GPIO_SET_LOW(GPIO_SCLK_PORT, GPIO_SCLK_PIN);
    (data & 0x40) ? GPIO_SET_HIGH(GPIO_SDIO_PORT, GPIO_SDIO_PIN) : GPIO_SET_LOW(GPIO_SDIO_PORT, GPIO_SDIO_PIN);
    NOP_DELAY_US(2);
    GPIO_SET_HIGH(GPIO_SCLK_PORT, GPIO_SCLK_PIN);

    GPIO_SET_LOW(GPIO_SCLK_PORT, GPIO_SCLK_PIN);
    (data & 0x20) ? GPIO_SET_HIGH(GPIO_SDIO_PORT, GPIO_SDIO_PIN) : GPIO_SET_LOW(GPIO_SDIO_PORT, GPIO_SDIO_PIN);
    NOP_DELAY_US(2);
    GPIO_SET_HIGH(GPIO_SCLK_PORT, GPIO_SCLK_PIN);

    GPIO_SET_LOW(GPIO_SCLK_PORT, GPIO_SCLK_PIN);
    (data & 0x10) ? GPIO_SET_HIGH(GPIO_SDIO_PORT, GPIO_SDIO_PIN) : GPIO_SET_LOW(GPIO_SDIO_PORT, GPIO_SDIO_PIN);
    NOP_DELAY_US(2);
    GPIO_SET_HIGH(GPIO_SCLK_PORT, GPIO_SCLK_PIN);

    GPIO_SET_LOW(GPIO_SCLK_PORT, GPIO_SCLK_PIN);
    (data & 0x08) ? GPIO_SET_HIGH(GPIO_SDIO_PORT, GPIO_SDIO_PIN) : GPIO_SET_LOW(GPIO_SDIO_PORT, GPIO_SDIO_PIN);
    NOP_DELAY_US(2);
    GPIO_SET_HIGH(GPIO_SCLK_PORT, GPIO_SCLK_PIN);

    GPIO_SET_LOW(GPIO_SCLK_PORT, GPIO_SCLK_PIN);
    (data & 0x04) ? GPIO_SET_HIGH(GPIO_SDIO_PORT, GPIO_SDIO_PIN) : GPIO_SET_LOW(GPIO_SDIO_PORT, GPIO_SDIO_PIN);
    NOP_DELAY_US(2);
    GPIO_SET_HIGH(GPIO_SCLK_PORT, GPIO_SCLK_PIN);

    GPIO_SET_LOW(GPIO_SCLK_PORT, GPIO_SCLK_PIN);
    (data & 0x02) ? GPIO_SET_HIGH(GPIO_SDIO_PORT, GPIO_SDIO_PIN) : GPIO_SET_LOW(GPIO_SDIO_PORT, GPIO_SDIO_PIN);
    NOP_DELAY_US(2);
    GPIO_SET_HIGH(GPIO_SCLK_PORT, GPIO_SCLK_PIN);

    GPIO_SET_LOW(GPIO_SCLK_PORT, GPIO_SCLK_PIN);
    (data & 0x01) ? GPIO_SET_HIGH(GPIO_SDIO_PORT, GPIO_SDIO_PIN) : GPIO_SET_LOW(GPIO_SDIO_PORT, GPIO_SDIO_PIN);
    NOP_DELAY_US(2);
    GPIO_SET_HIGH(GPIO_SCLK_PORT, GPIO_SCLK_PIN);

    NOP_DELAY_US(4);
}

// Read a byte of data from a register on the sensor.
uint8_t pmw3320_read_reg(uint8_t reg_addr) {
    pmw3320_cs_select();

    pmw3320_serial_write(reg_addr);
    NOP_DELAY_US(PMW3320_T_WUS_US);

    uint8_t byte = pmw3320_serial_read();
    NOP_DELAY_US(PMW3320_T_WUS_US);

    pmw3320_cs_deselect();

    return byte;
}

void pmw3320_write_reg(uint8_t reg_addr, uint8_t data) {
    pmw3320_cs_select();
    pmw3320_serial_write(0x80 | reg_addr);
    NOP_DELAY_US(PMW3320_T_WUS_US);

    pmw3320_serial_write(data);
    NOP_DELAY_US(PMW3320_T_WUS_US);

    pmw3320_cs_deselect();
}

report_pmw3320_t pmw3320_read_burst(void) {
    pmw3320_cs_select();

    report_pmw3320_t data;
    data.dx = 0;
    data.dy = 0;

    pmw3320_serial_write(REG_Motion_Burst);
    NOP_DELAY_US(PMW3320_T_WUS_US);

    uint8_t motion_reg = pmw3320_serial_read();
    NOP_DELAY_US(PMW3320_T_WUS_US);
    uint8_t y = pmw3320_serial_read();
    NOP_DELAY_US(PMW3320_T_WUS_US);
    uint8_t x = pmw3320_serial_read();
    NOP_DELAY_US(PMW3320_T_WUS_US);

    pmw3320_cs_deselect();

    if (motion_reg & 0x80) {
        data.dx = convert_twoscomp(x);
        data.dy = convert_twoscomp(y);
    } else {
        data.dx = 0;
        data.dy = 0;
    }

    return data;
}


int8_t convert_twoscomp(uint8_t data) {
    if ((data & 0x80) == 0x80)
        return -128 + (data & 0x7F);
    else
        return data;
}

uint16_t pmw3320_get_cpi(void) {
    uint8_t cpival = pmw3320_read_reg(REG_Resolution);
    // 0x1F is an inversion of 0x20 which is 0b100000
    return (uint16_t)((cpival & 0x1F) * PMW3320_CPI_STEP);
}

void pmw3320_set_cpi(uint16_t cpi) {
    uint8_t cpival = constrain((cpi / PMW3320_CPI_STEP), (PMW3320_CPI_MIN / PMW3320_CPI_STEP), (PMW3320_CPI_MAX / PMW3320_CPI_STEP)) - 1U;
    // Fifth bit is probably a control bit.
    // PMW3320 datasheet don't have any info on this, so this is a pure guess.
    pmw3320_write_reg(REG_Resolution, 0x20 | cpival);
}

bool __attribute__((weak)) pmw3320_check_signature(void) {
    uint8_t pid  = pmw3320_read_reg(REG_Product_ID);
    uint8_t pid2 = pmw3320_read_reg(REG_Inverse_Product_ID);

    return (pid == 0x3b && pid2 == 0xc4);
}

report_mouse_t pmw3320_get_report(report_mouse_t mouse_report) {
    report_pmw3320_t data = pmw3320_read_burst();

    if (data.dx != 0 || data.dy != 0) {
        pd_dprintf("Raw ] X: %d, Y: %d\n", data.dx, data.dy);
        mouse_report.x = (mouse_xy_report_t)data.dx;
        mouse_report.y = (mouse_xy_report_t)data.dy;
    }

    return mouse_report;
}

#if defined(VIAL_ENABLE)
void pointing_device_driver_init(void){
    pmw3320_init();
}
#else
bool pointing_device_driver_init(void){
    return pmw3320_init();
}
#endif


report_mouse_t pointing_device_driver_get_report(report_mouse_t mouse_report) {
    return pmw3320_get_report(mouse_report);
}

uint16_t pointing_device_driver_get_cpi(void) {
    return pmw3320_get_cpi();
}

void pointing_device_driver_set_cpi(uint16_t cpi) {
    pmw3320_set_cpi(cpi);
}

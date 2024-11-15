// Copyright 2024 Dasky (@daskygit)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#define POINTING_DEVICE_MOTION_PIN C14
#define POINTING_DEVICE_MOTION_PIN_ACTIVE_LOW
#define DEBUG_MATRIX_SCAN_RATE

#define I2C_DRIVER I2CD1
#define I2C1_SCL_PIN B6
#define I2C1_SDA_PIN B7
#define I2C1_SCL_PAL_MODE 1
#define I2C1_SDA_PAL_MODE 1
#define I2C1_TIMINGR_PRESC 0x00U
#define I2C1_TIMINGR_SCLDEL 0x03U
#define I2C1_TIMINGR_SDADEL 0x01U
#define I2C1_TIMINGR_SCLH 0x03U
#define I2C1_TIMINGR_SCLL 0x09U

#define AW21036_GCR4 0x07 // Slowest LED output slew rate for rise and fall
#define AW21036_PHCR 0x80 // Enable PWM phase delay
#define AW21036_SSCR 0x1C // Enable 35% spread spectrum range
#define AW21036_GCCR 0xFF // Maximum global current
#define AW21036_WBR 0x37  // Red scaling for white balance
#define AW21036_WBG 0x37  // Green scaling for white balance
#define AW21036_WBB 0xFF  // Blue scaling for white balance

#define AW21036_COL_ADJUSTMENTS 30 // Define the amount of COLs to adjust
#define AW21036_ENABLE_PIN A6

#ifndef AW21036_LED_COUNT
#    define AW21036_LED_COUNT 10
#endif

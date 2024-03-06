// Copyright 2024 Dasky (@Dasky)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#define PS2_RDY GP28
#define PS2_DATA GP29
#define PS2_CLK GP6

#define TPS_RDY GP4
#define TPS_SDA GP2
#define TPS_SCL GP3

#define CIRQUE_SS GP21
#define CIRQUE_MOSI GP23
#define CIRQUE_MISO GP20
#define CIRQUE_CLK GP22
#define CIRQUE_RDY GP26

#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 200U
#define SERIAL_USART_FULL_DUPLEX
//#define SERIAL_USART_TX_PIN GP0
#define SERIAL_USART_RX_PIN GP1
#define SERIAL_USART_PIN_SWAP
#define I2C_DRIVER I2CD1
#define I2C1_SDA_PIN GP2
#define I2C1_SCL_PIN GP3

#define AZOTEQ_IQS5XX_TPS65
#define SPLIT_POINTING_ENABLE
#define POINTING_DEVICE_COMBINED

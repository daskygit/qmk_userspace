// Copyright 2024 Dasky (@daskygit)
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#define OVERRIDE_DEBOUNCE 7
#define OVERRIDE_RGB_MATRIX_BRIGHTNESS 255

#define MOUSE_EXT_REPORT
// #define EE_HANDS
#define PMW33XX_CS_PIN POINTING_DEVICE_CS_PIN

// #define SERIAL_USART_SPEED 500000

#define BOOTMAGIC_COLUMN_RIGHT 0
#define BOOTMAGIC_ROW_RIGHT 4
#define SPLIT_HAND_MATRIX_GRID GP8, GP9

#define PMW33XX_MOTION_PIN GP27
// R1 GP29 - motion
// C1 GP27 - tx/rx

#define USB_VBUS_PIN GP19

#define SERIAL_USART_SPEED 2000000
#define SERIAL_USART_TIMEOUT 5
#define SERIAL_USART_FULL_DUPLEX
#define SERIAL_USART_RX_PIN GP29
#define SERIAL_USART_PIN_SWAP

#undef POINTING_DEVICE_RIGHT
#undef POINTING_DEVICE_INVERT_X
#undef ROTATIONAL_TRANSFORM_ANGLE

#define POINTING_DEVICE_INVERT_Y
#define POINTING_DEVICE_INVERT_X_RIGHT
#define ROTATIONAL_TRANSFORM_ANGLE 0
#define POINTING_DEVICE_COMBINED
#define POINTING_DEVICE_HIRES_SCROLL_ENABLE
#define WHEEL_EXTENDED_REPORT
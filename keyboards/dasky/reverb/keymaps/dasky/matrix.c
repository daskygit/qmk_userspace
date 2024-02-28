// Copyright 2023 Dasky (@daskygit)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "quantum.h"
#include "gpio.h"
#include "string.h"
#include "print.h"

const pin_t matrix_read_pins[MATRIX_COLS] = {GP12,GP1,GP4,GP13,GP6,GP8,GP27,GP22};
const pin_t matrix_out_pins[MATRIX_ROWS]  = {GP11,GP7,GP9,GP28,GP26, GP10, GP14};

#define READ_BITMASK (1 << 12 | 1 << 1 | 1 << 4 | 1 << 13 | 1 << 6 | 1 << 8 | 1 << 27 | 1 << 22)
#define OUT_BITMASK (1 << 11 | 1 << 7 | 1 << 9 | 1 << 28 | 1 << 26 | 1 << 11 | 1 << 14)


void matrix_init_custom(void) {
    for (uint8_t i = 0; i < MATRIX_ROWS; i++){
        setPinOutputPushPull(matrix_out_pins[i]);
        writePinHigh(matrix_out_pins[i]);
    }
    for (uint8_t i = 0; i < MATRIX_COLS; i++){
        setPinInputHigh(matrix_read_pins[i]);
    }
}

bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    bool                matrix_has_changed           = false;
    static matrix_row_t temp_matrix[MATRIX_ROWS]     = {0};

    for (uint8_t current_row = 0; current_row < MATRIX_ROWS; current_row++) {
        pin_t output_pin = matrix_out_pins[current_row];
        SIO->GPIO_OUT_CLR = 1 << output_pin;
        while ((SIO->GPIO_IN & (1 << output_pin)) != 0)
            ;
        uint32_t gpio_port = SIO->GPIO_IN;
        SIO->GPIO_OUT_SET = 1 << output_pin;
        while ((SIO->GPIO_IN & (READ_BITMASK | OUT_BITMASK)) != (READ_BITMASK | OUT_BITMASK))
            ;
        matrix_row_t row_status = (gpio_port & (1 << 12)) >> 12  | (gpio_port & (1 << 1)) << 0 | (gpio_port & (1 << 4)) >> 2 | (gpio_port & (1 << 13)) >> 10 | (gpio_port & (1 << 6)) >> 2 | (gpio_port & (1 << 8)) >> 3 | (gpio_port & (1 << 27)) >> 21 | (gpio_port & (1 << 22)) >> 15;
        temp_matrix[current_row] = ~(row_status);
    }

    if (memcmp(temp_matrix, current_matrix, sizeof(temp_matrix))) {
        memcpy(current_matrix, temp_matrix, sizeof(temp_matrix));
        matrix_has_changed = true;
    }

    return matrix_has_changed;
}
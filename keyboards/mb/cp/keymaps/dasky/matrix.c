// Copyright 2024 Dasky (@daskygit)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "quantum.h"
#include "gpio.h"
#include "string.h"
#include "print.h"

const pin_t matrix_read_pins[MATRIX_COLS] = {GP17,GP18,GP19,GP20,GP21,GP16};
const pin_t matrix_out_pins[MATRIX_ROWS]  = {GP4,GP5,GP8,GP9};

#define READ_BITMASK (1 << 17 | 1 << 18 | 1 << 19 | 1 << 20 | 1 << 21 | 1 << 16)
#define OUT_BITMASK (1 << 4 | 1 << 5 | 1 << 8 | 1 << 9)


void matrix_init_custom(void) {
    for (uint8_t i = 0; i < MATRIX_ROWS/2; i++){
        gpio_set_pin_output_push_pull(matrix_out_pins[i]);
        gpio_write_pin_high(matrix_out_pins[i]);
    }
    for (uint8_t i = 0; i < MATRIX_COLS; i++){
        gpio_set_pin_input_high(matrix_read_pins[i]);
    }
}

bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    bool                matrix_has_changed           = false;
    static matrix_row_t temp_matrix[MATRIX_ROWS/2]     = {0};

    for (uint8_t current_row = 0; current_row < MATRIX_ROWS/2; current_row++) {
        pin_t output_pin = matrix_out_pins[current_row];
        SIO->GPIO_OUT_CLR = 1 << output_pin;
        while ((SIO->GPIO_IN & (1 << output_pin)) != 0)
            ;
        uint32_t gpio_port = SIO->GPIO_IN;
        SIO->GPIO_OUT_SET = 1 << output_pin;
        while ((SIO->GPIO_IN & (READ_BITMASK | OUT_BITMASK)) != (READ_BITMASK | OUT_BITMASK))
            ;
        matrix_row_t row_status = (gpio_port & (1 << 17)) >> 17  | (gpio_port & (1 << 18)) >> 17 | (gpio_port & (1 << 19)) >> 17 | (gpio_port & (1 << 20)) >> 17 | (gpio_port & (1 << 21)) >> 17 | (gpio_port & (1 << 16)) >> 11;
        temp_matrix[current_row] = ~(row_status);
    }

    if (memcmp(temp_matrix, current_matrix, sizeof(temp_matrix))) {
        memcpy(current_matrix, temp_matrix, sizeof(temp_matrix));
        matrix_has_changed = true;
    }

    return matrix_has_changed;
}

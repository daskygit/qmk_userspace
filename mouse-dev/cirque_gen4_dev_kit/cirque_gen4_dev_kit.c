// Copyright 2024 Dasky (@daskygit)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "quantum.h"

void board_init(void) {
    gpio_set_pin_output(B17);
    gpio_write_pin_high(B17); // Enable PSU (IO1)
    gpio_set_pin_output(D1);
    gpio_write_pin_high(D1); // Enable I2C pullups (IO14)
}

// fix pins because kinetis
void i2c_init(void) {
    static bool is_initialised = false;
    if (!is_initialised) {
        is_initialised = true;

        // Try releasing special pins for a short time
        palSetLineMode(I2C1_SCL_PIN, PAL_MODE_INPUT);
        palSetLineMode(I2C1_SDA_PIN, PAL_MODE_INPUT);

        chThdSleepMilliseconds(10);
        palSetLineMode(I2C1_SCL_PIN, I2C1_SCL_PAL_MODE);
        palSetLineMode(I2C1_SDA_PIN, I2C1_SDA_PAL_MODE);
        PORTB->PCR[0] |= PORTx_PCRn_ODE;
        PORTB->PCR[1] |= PORTx_PCRn_ODE;
    }
}

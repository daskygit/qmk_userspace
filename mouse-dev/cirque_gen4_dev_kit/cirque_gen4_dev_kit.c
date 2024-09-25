// Copyright 2024 Dasky (@daskygit)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "quantum.h"

void board_init(void) {
    gpio_set_pin_output(B17);
    gpio_write_pin_high(B17); // Enable PSU (IO1)
    gpio_set_pin_output(D1);
    gpio_write_pin_high(D1); // Enable I2C pullups (IO14)
}

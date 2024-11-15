// Copyright 2024 Dasky (@Dasky)
// SPDX-License-Identifier: GPL-2.0-or-later
#include "hal.h"

void board_init(void) {
    SYSCFG->CFGR1 |= SYSCFG_CFGR1_I2C1_DMA_RMP;
}

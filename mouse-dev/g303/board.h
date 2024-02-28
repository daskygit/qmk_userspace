// Copyright 2023 Dasky (@Dasky)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include_next <board.h>

#undef STM32L152xE
#define STM32L151xB
#undef STM32_HSE_BYPASS

#undef STM32_HSECLK
#define STM32_HSECLK 8000000

#define STM32L1XX_PROD_CAT 1
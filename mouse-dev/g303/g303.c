// Copyright 2023 Dasky (@Dasky)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "color.h"
#include "quantum.h"
#include <ch.h>
#include <hal.h>

__attribute__((weak)) void bootloader_jump(void) {
  RTC->BKP1R = 0x544F4F42UL;
  NVIC_SystemReset();
}

void mcu_reset(void) { NVIC_SystemReset(); }

void board_init(void) {
  setPinOutput(PMW_LED_PIN);
  writePin(PMW_LED_PIN, 0);
  setPinOutput(PMW_RESET_PIN);
  writePin(PMW_RESET_PIN, 1);
  setPinOutput(ENC_LED_PIN);
  writePin(ENC_LED_PIN, 1);

  setPinOutput(DP_PU_PIN);
  writePin(DP_PU_PIN, 0);

  setPinOutput(RED_G_PIN); // RED G
  writePin(RED_G_PIN, 1);
  setPinOutput(GREEN_G_PIN); // GREEN G
  writePin(GREEN_G_PIN, 1);
  setPinOutput(BLUE_G_PIN); // BLUE G
  writePin(BLUE_G_PIN, 1);

  setPinOutput(GREEN_SIDE_PIN); // GREEN SIDE
  writePin(GREEN_SIDE_PIN, 0);
  setPinOutput(RED_SIDE_PIN); // RED SIDE
  writePin(RED_SIDE_PIN, 0);
  setPinOutput(BLUE_SIDE_PIN); // BLUE SIDE
  writePin(BLUE_SIDE_PIN, 1);

  setPinInput(A8);
  setPinInputHigh(B2);

}
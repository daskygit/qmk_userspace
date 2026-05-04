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
  gpio_set_pin_output_push_pull(PMW_LED_PIN);
  gpio_write_pin_low(PMW_LED_PIN);
  gpio_set_pin_output_push_pull(PMW_RESET_PIN);
  gpio_write_pin_high(PMW_RESET_PIN);
  gpio_set_pin_output_push_pull(ENC_LED_PIN);
  gpio_write_pin_high(ENC_LED_PIN);

  gpio_set_pin_output_push_pull(DP_PU_PIN);
  gpio_write_pin_low(DP_PU_PIN);

  gpio_set_pin_output_push_pull(RED_G_PIN); // RED G
  gpio_write_pin_high(RED_G_PIN);
  gpio_set_pin_output_push_pull(GREEN_G_PIN); // GREEN G
  gpio_write_pin_high(GREEN_G_PIN);
  gpio_set_pin_output_push_pull(BLUE_G_PIN); // BLUE G
  gpio_write_pin_high(BLUE_G_PIN);

  gpio_set_pin_output_push_pull(GREEN_SIDE_PIN); // GREEN SIDE
  gpio_write_pin_low(GREEN_SIDE_PIN);
  gpio_set_pin_output_push_pull(RED_SIDE_PIN); // RED SIDE
  gpio_write_pin_low(RED_SIDE_PIN);
  gpio_set_pin_output_push_pull(BLUE_SIDE_PIN); // BLUE SIDE
  gpio_write_pin_high(BLUE_SIDE_PIN);

  gpio_set_pin_input(A8);
  gpio_set_pin_input_high(B2);

}

// Copyright 2023 Dasky (@Dasky)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "quantum.h"

typedef enum {
  LD7032_REG_SOFTRES = 0x01,
  LD7032_REG_DISP_ON_OFF = 0x02,
  LD7032_REG_DATA_RW = 0x08,
  LD7032_REG_DISP_DIRECTION = 0x09,
  LD7032_REG_IFMODE = 0x0D,
  LD7032_REG_PEAK_WIDTH = 0x10,
  LD7032_REG_DOT_CURRENT = 0x12,
  LD7032_REG_SCAN_MODE = 0x13,
  LD7032_REG_DISP_STBY_ON_OFF = 0x14,
  LD7032_REG_PEAK_DELAY = 0x16,
  LD7032_REG_ROW_SCAN = 0x17,
  LD7032_REG_PRE_C_WIDTH = 0x18,
  LD7032_REG_DFRAME = 0x1A,
  LD7032_REG_DATA_REVERSE = 0x1C,
  LD7032_REG_WRITE_DIRECTION = 0x1D,
  LD7032_REG_READREG = 0x20,
  LD7032_REG_DISP_SIZE_X = 0x30,
  LD7032_REG_DISP_SIZE_Y = 0x32,
  LD7032_REG_X_BOX_ADR_START = 0x34,
  LD7032_REG_X_BOX_ADR_END = 0x35,
  LD7032_REG_Y_BOX_ADR_START = 0x36,
  LD7032_REG_Y_BOX_ADR_END = 0x37,
  LD7032_REG_X_DISP_START = 0x38,
  LD7032_REG_Y_DISP_START = 0x39,
  LD7032_REG_XTALK_EN = 0x3A,
  LD7032_REG_XTALK_REF = 0x3B,
  LD7032_REG_AGING_EN = 0x3C,
  LD7032_REG_VDD_SEL = 0x3D,
  LD7032_REG_TESTCNT0 = 0x3E,
  LD7032_REG_VCC_R_SEL = 0x3F,
  LD7032_REG_PRE_C_SELECT = 0x44,
  LD7032_REG_ROW_OVERLAP = 0x48,
  LD7032_REG_S_SLEEP_TIMER = 0xC0,
  LD7032_REG_S_SLEEP_START = 0xC2,
  LD7032_REG_S_STEP_TIMER = 0xC3,
  LD7032_REG_S_STEP_UNIT = 0xC4,
  LD7032_REG_S_CONDITION = 0xCC,
  LD7032_REG_S_START_STOP = 0xCD,
  LD7032_REG_S_SELECT = 0xCE,
  LD7032_REG_TESTCNT1 = 0xF0, //-0xFF
} ld7032_regs;

typedef struct {
  bool right : 1;          // default - false
  bool up : 1;             // default - false
  bool vertical : 1;       // default - false
  bool flip_bit_order : 1; //????
  uint8_t _unsed : 4;
} ld7032_write_direction_t;

const ld7032_write_direction_t direction = {
    .right = true, .vertical = false, .flip_bit_order = true, .up = true};

#include "i2c_master.h"

void ld7032_init(void) {
    i2c_init();
}

void ld7032_test(void) {
    uint8_t data[2] = {0};

    i2c_writeReg(0x37 << 1, LD7032_REG_DISP_STBY_ON_OFF, &data[0], 1, 10);

    i2c_writeReg(0x37 << 1, LD7032_REG_DISP_ON_OFF, &data[0], 1, 10);

    data[0] = 0x5;
    i2c_writeReg(0x37 << 1, LD7032_REG_DFRAME, &data[0], 1, 10);

    i2c_writeReg(0x37 << 1, LD7032_REG_WRITE_DIRECTION, (uint8_t *)&direction, 1, 10);

    data[0] = 0x0;
    i2c_writeReg(0x37 << 1, LD7032_REG_DISP_DIRECTION, &data[0], 1, 10);

    data[0] = 0 + 16;
    data[1] = 0x5f + 16;
    i2c_writeReg(0x37 << 1, LD7032_REG_DISP_SIZE_X, &data[0], 2, 10);

    data[0] = 0 + 16;
    data[1] = 23 + 16;
    i2c_writeReg(0x37 << 1, LD7032_REG_DISP_SIZE_Y, &data[0], 2, 10);

    data[0] = 0x70;
    i2c_writeReg(0x37 << 1, LD7032_REG_X_DISP_START, &data[0], 1, 10);

    data[0] = 0x19;
    i2c_writeReg(0x37 << 1, LD7032_REG_Y_DISP_START, &data[0], 1, 10);

    data[0] = 0x1F;
    i2c_writeReg(0x37 << 1, LD7032_REG_PEAK_WIDTH, &data[0], 1, 10);

    data[0] = 0x05;
    i2c_writeReg(0x37 << 1, LD7032_REG_PEAK_DELAY, &data[0], 1, 10);

    // data[0] = 16;
    // i2c_writeReg(0x37 << 1, LD7032_REG_PRE_C_WIDTH, &data[0], 1, 10);

    data[0] = 0x1;
    i2c_writeReg(0x37 << 1, LD7032_REG_SCAN_MODE, &data[0], 1, 10);

    data[0] = 0x1f; // 3f
    i2c_writeReg(0x37 << 1, LD7032_REG_DOT_CURRENT, &data[0], 1, 10);

    data[0] = 0x01;
    i2c_writeReg(0x37 << 1, LD7032_REG_VDD_SEL, &data[0], 1, 10);

    const uint8_t empty[128 * 40 / 8] = {0};
    i2c_writeReg(0x37 << 1, LD7032_REG_DATA_RW, &empty[0], sizeof(empty), 200);

    data[0] = 0x0;
    i2c_writeReg(0x37 << 1, LD7032_REG_X_BOX_ADR_START, &data[0], 1, 10);

    data[0] = 0x0B; //
    i2c_writeReg(0x37 << 1, LD7032_REG_X_BOX_ADR_END, &data[0], 1, 10);

    data[0] = 0x0;
    i2c_writeReg(0x37 << 1, LD7032_REG_Y_BOX_ADR_START, &data[0], 1, 10);

    data[0] = 0x18;
    i2c_writeReg(0x37 << 1, LD7032_REG_Y_BOX_ADR_END, &data[0], 1, 10);

    // clang-format off
  static const uint8_t prime[]={
     0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00,  0x00, 0x00, 0x00,
     0x01, 0x24, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x01, 0x24, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x01, 0x24, 0x90, 0x1c, 0x65, 0x5e, 0xe7, 0xb8, 0x72, 0x43, 0x3e, 0x90,
     0x03, 0xff, 0xf8, 0x12, 0x95, 0x50, 0x94, 0x24, 0x4a, 0x44, 0xaa, 0x90,
     0x07, 0xff, 0xfc, 0x12, 0x95, 0x50, 0x94, 0x24, 0x4a, 0x44, 0xaa, 0xa0,
     0x3f, 0xff, 0xff, 0x92, 0x95, 0x5c, 0x97, 0x24, 0x73, 0xc4, 0xaa, 0xc0,
     0x07, 0x7b, 0xdc, 0x1c, 0x95, 0x50, 0xe4, 0x24, 0x48, 0x44, 0xaa, 0xa0,
     0x07, 0x7b, 0xdc, 0x10, 0x95, 0x50, 0x94, 0x24, 0x48, 0x45, 0x2a, 0x90,
     0x3f, 0x7b, 0xdf, 0x90, 0x62, 0x9e, 0x97, 0xb8, 0x73, 0xc2, 0xaa, 0x90,
     0x07, 0x7b, 0xdc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x07, 0x7b, 0xdc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x3f, 0x3b, 0x9f, 0x80, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00,
     0x07, 0x9b, 0x3c, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00,
     0x07, 0xc0, 0x7c, 0x00, 0x00, 0x18, 0xc6, 0x20, 0x87, 0xf8, 0x00, 0x00,
     0x3f, 0xfb, 0xff, 0x80, 0x00, 0x1b, 0x5b, 0x6a, 0xbf, 0x78, 0x00, 0x00,
     0x07, 0xfb, 0xfc, 0x00, 0x00, 0x1b, 0x5b, 0x6a, 0xbf, 0x78, 0x00, 0x00,
     0x07, 0xfb, 0xfc, 0x00, 0x00, 0x1b, 0x5b, 0x6a, 0x8c, 0x18, 0x00, 0x00,
     0x3f, 0xff, 0xff, 0x80, 0x00, 0x18, 0xc7, 0x6a, 0xbf, 0x78, 0x00, 0x00,
     0x07, 0xff, 0xfc, 0x00, 0x00, 0x1b, 0xdb, 0x6a, 0xbf, 0x78, 0x00, 0x00,
     0x03, 0xff, 0xf8, 0x00, 0x00, 0x1b, 0xda, 0x2a, 0x87, 0xf8, 0x00, 0x00,
     0x01, 0x24, 0x90, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00,
     0x01, 0x24, 0x90, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00,
     0x01, 0x24, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };
  // clang-format on

  i2c_writeReg(0x37 << 1, LD7032_REG_DATA_RW, &prime[0], sizeof(prime), 100);

  data[0] = 0x01;
  i2c_writeReg(0x37 << 1, LD7032_REG_DISP_ON_OFF, &data[0], 1, 10);

  wait_ms(2000);
  data[1] = 0x1F;
  i2c_writeReg(0x37 << 1, LD7032_REG_S_STEP_TIMER, &data[1], 1, 10);
  data[1] = 0x01;
  i2c_writeReg(0x37 << 1, LD7032_REG_S_STEP_UNIT, &data[1], 1, 10);
  i2c_writeReg(0x37 << 1, LD7032_REG_S_START_STOP, &data[0], 1, 10);

  // data[1] = 0xF;
  // i2c_writeReg(0x37 << 1, LD7032_REG_S_SLEEP_TIMER, &data[1], 1, 10);
  // i2c_writeReg(0x37 << 1, LD7032_REG_S_SLEEP_START, &data[0], 1, 10);

  // i2c_writeReg(0x37 << 1, LD7032_REG_DISP_ON_OFF, (uint8_t *)0, 1, 10);

  // i2c_stop();
}

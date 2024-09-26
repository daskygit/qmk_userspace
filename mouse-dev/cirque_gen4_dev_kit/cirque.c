// Copyright 2024 Dasky (@daskygit)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "i2c_master.h"
#include "pointing_device.h"
#include "util.h"
#include <string.h>

#define CIRQUE_RUSHMORE_ADDR (0x2A << 1)
#define CIRQUE_RUSHMORE_EXT_READ_ADDR 0x0901
#define CIRQUE_RUSHMORE_EXT_WRITE_ADDR 0x0900
#define CIRQUE_RUSHMORE_TIMEOUT 50

typedef struct PACKED {
    uint8_t length_low_byte;
    uint8_t length_high_byte;
    uint8_t report_id;
} cirque_rushmore_descriptor_t;

typedef struct PACKED {
    uint8_t touch_single_sample : 1;
    uint8_t touch_confidence : 1;
    uint8_t _unused : 1;
    uint8_t pen : 1;
    uint8_t __unused : 3;
    uint8_t palm_reject : 1;
} cirque_rushmore_palm_data_t;

typedef struct PACKED {
    cirque_rushmore_palm_data_t palm;
    uint8_t                     x_low;  // bottom 8 bits for 16-bit x value
    uint8_t                     x_high; // top
    uint8_t                     y_low;  // bottom 8 bits for 16-bit y value
    uint8_t                     y_high; // top
} cirque_rushmore_finger_data_t;

typedef struct PACKED {
    cirque_rushmore_descriptor_t  descriptor;
    uint8_t                       num_contacts;
    cirque_rushmore_finger_data_t finger_0_data;
    cirque_rushmore_finger_data_t finger_1_data;
    cirque_rushmore_finger_data_t finger_2_data;
    cirque_rushmore_finger_data_t finger_3_data;
    cirque_rushmore_finger_data_t finger_4_data;
    uint8_t                       buttons_data;
    uint8_t                       unused[23];
} cirque_rushmore_absolute_position_data_t;

_Static_assert(sizeof(cirque_rushmore_absolute_position_data_t) == 53, "cirque_rushmore_absolute_position_data_t should be 53 bytes");

typedef struct PACKED {
    cirque_rushmore_descriptor_t descriptor;
    uint8_t                      buttons;
    int8_t                       x_delta;
    int8_t                       y_delta;
    int8_t                       scroll_delta;
    int8_t                       pan_delta;
} cirque_rushmore_mouse_packet_t;

typedef struct PACKED {
    bool left_ctrl : 1;
    bool left_shift : 1;
    bool left_alt : 1;
    bool left_gui : 1;
    bool right_ctrl : 1;
    bool right_shift : 1;
    bool right_alt : 1;
    bool right_gui : 1;
} cirque_rushmore_modifiers_t;

typedef struct PACKED {
    cirque_rushmore_descriptor_t descriptor;
    cirque_rushmore_modifiers_t  modifiers;
    uint8_t                      reserved;
    uint8_t                      keycode_1;
    uint8_t                      keycode_2;
    uint8_t                      keycode_3;
    uint8_t                      keycode_4;
    uint8_t                      keycode_5;
    uint8_t                      keycode_6;
} cirque_rushmore_keyboard_packet_t;

typedef enum {
    cirque_rushmore_menu        = 0x00,
    cirque_rushmore_tab         = 0x2b,
    cirque_rushmore_letter_d    = 0x07,
    cirque_rushmore_right_arrow = 0x4f,
    cirque_rushmore_left_arrow  = 0x50,
} cirque_rushmore_keycodes_t;

uint8_t cirque_rushmore_checksum(uint8_t length, uint8_t *data) {
    uint8_t return_checksum = 0;
    for (uint8_t i = 0; i < length; i++) {
        return_checksum += *(data + i);
    }
    return return_checksum;
}

i2c_status_t cirque_rushmore_extended_write(uint32_t extended_address, uint16_t length, uint8_t *data) {
    uint8_t packet[length + 9];
    packet[0] = 0x0900 & 0xff;
    packet[1] = (0x0900 >> 8) & 0xff;
    packet[2] = extended_address & 0xff;
    packet[3] = (extended_address >> 8) & 0xff;
    packet[4] = (extended_address >> 16) & 0xff;
    packet[5] = (extended_address >> 24) & 0xff;
    packet[6] = length & 0xff;
    packet[7] = (length >> 8) & 0xff;
    memcpy(&packet[8], data, length);
    packet[length + 8] = cirque_rushmore_checksum(length + 8, packet);
    return i2c_transmit((CIRQUE_RUSHMORE_ADDR), packet, sizeof(packet), CIRQUE_RUSHMORE_TIMEOUT);
}

i2c_status_t cirque_rushmore_extended_read(uint32_t extended_address, uint16_t length, uint8_t *data) {
    i2c_status_t status = I2C_STATUS_ERROR;
    uint8_t      write_packet[8];
    write_packet[0] = 0x0901 & 0xff;
    write_packet[1] = (0x0901 >> 8) & 0xff;
    write_packet[2] = extended_address & 0xff;
    write_packet[3] = (extended_address >> 8) & 0xff;
    write_packet[4] = (extended_address >> 16) & 0xff;
    write_packet[5] = (extended_address >> 24) & 0xff;
    write_packet[6] = length & 0xff;
    write_packet[7] = (length >> 8) & 0xff;
    status          = i2c_transmit((CIRQUE_RUSHMORE_ADDR), write_packet, sizeof(write_packet), CIRQUE_RUSHMORE_TIMEOUT);

    if (status == I2C_STATUS_SUCCESS) {
        uint8_t read_packet[length + 3];

        status = i2c_receive((CIRQUE_RUSHMORE_ADDR) | 1, read_packet, sizeof(read_packet), CIRQUE_RUSHMORE_TIMEOUT);

        if (read_packet[length - 1] != cirque_rushmore_checksum(length, read_packet)) {
            status = I2C_STATUS_ERROR;
        }
        memcpy(data, &read_packet[2], length);
    }

    return status;
}

i2c_status_t cirque_rushmore_get_report(report_mouse_t *mouse_report) {
    cirque_rushmore_descriptor_t descriptor;
    i2c_read_register16(CIRQUE_RUSHMORE_ADDR, 0x0001, (uint8_t *)&descriptor, sizeof(descriptor), CIRQUE_RUSHMORE_TIMEOUT);
    if (descriptor.report_id == 6) {
        cirque_rushmore_mouse_packet_t mouse;
        i2c_read_register16(CIRQUE_RUSHMORE_ADDR, 0x0001, (uint8_t *)&mouse, sizeof(mouse), CIRQUE_RUSHMORE_TIMEOUT);
        if (mouse.descriptor.report_id == 6) {
            mouse_report->buttons = mouse.buttons;
            mouse_report->x       = mouse.x_delta;
            mouse_report->y       = mouse.y_delta;
            mouse_report->h       = mouse.pan_delta;
            mouse_report->v       = mouse.scroll_delta;
        }
    } else if (descriptor.report_id == 8) {
        cirque_rushmore_keyboard_packet_t keyboard;
        i2c_read_register16(CIRQUE_RUSHMORE_ADDR, 0x0001, (uint8_t *)&keyboard, sizeof(keyboard), CIRQUE_RUSHMORE_TIMEOUT);
        // do nothing with keycode report for the moment, just need to clear the queue
    }
    return I2C_STATUS_SUCCESS;
}

void pointing_device_driver_init(void) {
    i2c_init();
    // Write 0x03 in register 0xC2C4 to enter Absolute mode.
    // Write 0x01 to register 0xC2C4 to enter Relative mode.
    // uint8_t mode = 0x3;
    // cirque_rushmore_extended_write(0xC2C4, 1, &mode);
    // uint8_t data;
    // cirque_rushmore_extended_read(0xC2C4, 1, &data);
    // uint8_t data[2];
    // cirque_rushmore_extended_read(0xC2D2, 2, data);
}

report_mouse_t pointing_device_driver_get_report(report_mouse_t mouse_report) {
    // i2c_readReg16(devaddr, regaddr, data, length, timeout) return mouse_report;
    cirque_rushmore_get_report(&mouse_report);
    return mouse_report;
}
uint16_t pointing_device_driver_get_cpi(void) {
    return 0;
}
void pointing_device_driver_set_cpi(uint16_t cpi) {}

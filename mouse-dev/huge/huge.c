// Copyright 2026 Dasky
// SPDX-License-Identifier: GPL-2.0-or-later
#include QMK_KEYBOARD_H

bool dip_switch_update_kb(uint8_t index, bool active) {
    if (!dip_switch_update_user(index, active)) {
        return false;
    }

    if (active) {
        switch (index) {
            case 0:
                pointing_device_set_cpi(800);
                break;

            case 1:
                pointing_device_set_cpi(1600);
                break;

            case 2:
                pointing_device_set_cpi(3200);
                break;
        }
    }
    return true;
}

void pointing_device_init_kb(void){
    dip_switch_read(true);
}

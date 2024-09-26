/* Copyright 2019
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#define I2C1_SCL_PIN B2
#define I2C1_SDA_PIN B3
#define I2C1_SCL_PAL_MODE PAL_MODE_ALTERNATIVE_2
#define I2C1_SDA_PAL_MODE PAL_MODE_ALTERNATIVE_2
#define I2C1_CLOCK_SPEED 400000
// #define POINTING_DEVICE_TASK_THROTTLE_MS 1
#define POINTING_DEVICE_MOTION_PIN C3
#define POINTING_DEVICE_MOTION_PIN_ACTIVE_LOW

#define DIGITIZER_MOTION_PIN C3
#define DIGITIZER_MOTION_PIN_ACTIVE_LOW

#define DIGITIZER_CONTACT_COUNT 5
#define DIGITIZER_WIDTH_MM 105
#define DIGITIZER_HEIGHT_MM 65

#define DIGITIZER_RESOLUTION_X (DIGITIZER_WIDTH_MM * 50)
#define DIGITIZER_RESOLUTION_Y (DIGITIZER_HEIGHT_MM * 50)

#define DEBUG_MATRIX_SCAN_RATE

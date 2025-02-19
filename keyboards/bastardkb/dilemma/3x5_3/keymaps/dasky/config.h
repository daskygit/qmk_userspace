#pragma once

#define MAXTOUCH_BOOTLOADER_GESTURE
#define SPLIT_DIGITIZER_ENABLE
#define DIGITIZER_RIGHT
#define DIGITIZER_TASK_THROTTLE_MS 1
#define DIGITIZER_CONTACT_COUNT 5
#undef SPLIT_POINTING_ENABLE
#undef POINTING_DEVICE_RIGHT

#define I2C_DRIVER I2CD1
#define I2C1_SDA_PIN GP2
#define I2C1_SCL_PIN GP3
#define I2C1_CLOCK_SPEED 1000000
#define DIGITIZER_MOTION_PIN GP12
#define DIGITIZER_MOTION_PIN_ACTIVE_LOW yes

#undef MAXTOUCH_DEBUG
#undef PROCYON_42_50
#define MXT_SWITCH_XY
#define MXT_INVERT_Y
#define MXT_GAIN 10
#define MXT_TOUCH_THRESHOLD 20
#define MXT_TOUCH_HYST 10
#define MXT_INTERNAL_TOUCH_THRESHOLD 10
#define MXT_INTERNAL_TOUCH_HYST 5
#define MXT_T56_SHIELDLESS_ENABLE
#define MXT_RETRANSMISSION_COMPENSATION_ENABLE 0
#define MXT_X_PITCH 38 // Procyon boards have a standard 3.8mm pitch
#define MXT_Y_PITCH 38
#define MXT_ACTIVE_ACQUISITION_INTERVAL 255 // Free run while active - this enables us to reach a polling rate of around 300Hz
#define MXT_CONFTHR 5                       // Increase debounce time
#define MXT_MOVE_HYSTERESIS_NEXT 16
#define MXT_SENSOR_WIDTH_MM 50
#define MXT_SENSOR_HEIGHT_MM 42
#define MXT_MATRIX_X_SIZE 12
#define MXT_MATRIX_Y_SIZE 10
#define MXT_MESALLOW 1 // This board has matrix size which prohibits self capacitance measurements
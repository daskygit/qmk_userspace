I2C_DRIVER_REQUIRED = yes

POINTING_DEVICE_ENABLE = yes
DIGITIZER_ENABLE = yes
DIGITIZER_DRIVER = custom
POINTING_DEVICE_DRIVER = digitizer

SRC += cirque.c

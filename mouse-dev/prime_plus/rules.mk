POINTING_DEVICE_DRIVERS += pmw3389
POINTING_DEVICE_DRIVER = pmw3389
POINTING_DEVICE_ENABLE = yes

DEBOUNCE_TYPE = asym_eager_defer_pk

QUANTUM_PAINTER_ENABLE = yes
ifeq ($(strip $(QUANTUM_PAINTER_ENABLE)), yes)
	QUANTUM_PAINTER_DRIVERS = ld7032_i2c
	VPATH += keyboards/steelseries/prime_plus/graphics
	SRC += sharetech11.qff.c
	SRC += primeplus.qgf.c
else
	LIB_SRC += i2c_master.c
	SRC += ld7032.c
endif

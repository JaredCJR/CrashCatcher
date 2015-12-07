PROJECT         := CrashingFPU_HexDump
DEVICES         := DISCO_F429ZI
GCC4MBED_DIR    := ../../../gcc4mbed
NO_FLOAT_SCANF  := 1
NO_FLOAT_PRINTF := 1
INCDIRS         := ../../include
LIBS_PREFIX     := ../../obj/armv7-m/Core/src/CrashCatcher_armv7m.o
LIBS_SUFFIX     := ../../lib/armv7-m/libCrashCatcher_HexDump_armv7m.a

MRI_INIT_PARAMETERS ?= "MRI_UART_1 MRI_UART_BAUD=230400"

include $(GCC4MBED_DIR)/build/gcc4mbed.mk

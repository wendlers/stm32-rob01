BINARY		 = firmware
OBJS			+= main.o board_stm32.o device_serial.o device_led.o device_dcmotor.o device_bumpersensor.o device_refsensor.o device_button.o
OBJS			+= shell.o shell_serial.o delay.o sensors.o actors.o drive_control.o shell_m2m.o config.o

include ../common.mk

check: $(SRC)
	$(CHECKER) $(CHECKERFLAGS) $(SRC)

gen-docs: $(HDR) $(SRC) 
	$(DOXYGEN) $(DOXYGENFLAGS)

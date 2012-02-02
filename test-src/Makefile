BINARY		 = firmware
OBJS			 = main.o test_shell.o 
OBJS			+= ../src/board_stm32.o ../src/device_serial.o ../src/device_led.o ../src/device_dcmotor.o 
OBJS			+= ../src/device_bumpersensor.o ../src/device_refsensor.o ../src/device_button.o ../src/shell.o ../src/shell_serial.o
OBJS			+= ../src/delay.o ../src/sensors.o ../src/actors.o ../src/drive_control.o

BINDIR		 = ../test-bin
INCDIR		 = -I../src/include

include ../common.mk

all:	project images

project:
	make -C ../src

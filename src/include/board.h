/*
 * This file is part of the RobotRemCtrlFW project.
 *
 * Copyright (C) 2011 Stefan Wendler <sw@kaltpost.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
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

#ifndef __BOARD_H_
#define __BOARD_H_

#include <libopencm3/stm32/systick.h>

#include <stdint.h>

#include "device.h"

/**
 * Define the board speed in MHz (needed to clac baudrate for USARTs)
 */
#define BOARD_SPEED_IN_MHZ		24

/**
 * The serial USART device
 */
extern device serial;

/**
 * The 1st status LED
 */
extern device status_led1;

/**
 * The 2nd status LED
 */
extern device status_led2;

/**
 * The left DC motor
 */
extern device dcmotor_left;

/**
 * The right DC motor
 */
extern device dcmotor_right;

/**
 * The left bumper sensor
 */
extern device bumpersensor_left;

/**
 * The right bumper sensor
 */
extern device bumpersensor_right;

/**
 * The left reflection sensor
 */
extern device refsensor_left;

/**
 * The right reflection sensor
 */
extern device refsensor_right;

/**
 * The 1st user button
 */
extern device button_user1;

/**
 * Holds all the devices available for the board.
 */
typedef struct {
     /**
      * Number of devices in the array
      */
     int size;

     /**
      * Array with device definitions. The array size
      * is given through the equally named member variable of this struct.
      */
     device_ref device_array[];
} board_devices;

/**
 * Initialize the board and the assigned hardware. See {@link board_stm32.c}.
 */
void board_init();

/**
 * Init the systick interrupt. Calls {@link sys_tick_handler}.
 *
 * @param[in]	clocksource	the clock source devider
 * @param[in]	reload		reload intervall
 */
void board_init_systick(uint8_t clocksource, uint32_t reload);

/**
 * Enable RTC. THe RTC gets updated every second.
 */
void board_init_rtc();

#endif

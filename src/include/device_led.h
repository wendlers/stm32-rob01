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

#ifndef __DEVICE_LED_H_
#define __DEVICE_LED_H_

#include <stdint.h>

#include "device.h"

/**
 * Initialization data for a LED.
 */
typedef struct {
     /**
      * Port for the LED. E.g. GPIOC.
      */
     uint32_t   port;

     /**
      * Pin for the LED. E.g. GPIO7.
      */
     uint16_t	 pin;

} device_data_led;

/**
 * Device init function for the LED.
 *
 * @param[in]	*data	the device data for the device to initialize
 */
void led_init(device_data *data);

/**
 * Turn the given LED device on.
 *
 * @param[in]	*dev	the device to work on
 */
void led_on(device *dev);

/**
 * Turn the given LED device off.
 *
 * @param[in]	*dev	the device to work on
 */
void led_off(device *dev);

/**
 * Toggle the given LED device.
 *
 * @param[in]	*dev	the device to work on
 */
void led_toggle(device *dev);

#endif

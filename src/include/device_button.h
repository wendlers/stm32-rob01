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

#ifndef __DEVICE_BUTTON_H_
#define __DEVICE_BUTTON_H_

#include <stdint.h>

#include "device.h"

/**
 * Initialization data for a button.
 */
typedef struct {
     /**
      * Port on which the output of the button is connected to (e.g GPIOA).
      */
     uint32_t   port;

     /**
      * Pin to which the output of the button is connected to (e.g GPIO9).
      */
     uint16_t	 pin;
} device_data_button;

/**
 * Device init function for the button. Initializes the port as "floating"
 *
 * @param[in]	*data	the device data for the device to initialize
 */
void button_init(device_data *data);

/**
 * Device init function for the button. Initializes the port with "pullup"
 *
 * @param[in]	*data	the device data for the device to initialize
 */
void button_init_pullup(device_data *data);

/**
 * Device init function for the button. Initializes the port with "pulldown"
 *
 * @param[in]	*data	the device data for the device to initialize
 */
void button_init_pulldown(device_data *data);

/**
 * Ask given button device if it is pressed.
 *
 * @param[in]	*dev	the button device to query
 * @return		1 pressed, 0 released
 */
int button_is_pressed(device *dev);

#endif

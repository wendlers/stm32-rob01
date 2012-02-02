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

#ifndef __DEVICE_BUMPERSENSOR_H_
#define __DEVICE_BUMPERSENSOR_H_

#include <stdint.h>

#include "device.h"

/**
 * Initialization data for a bumper sensro.
 */
typedef struct {
     /**
      * Port on which the output of the bumper sensor is connected to (e.g GPIOA).
      */
     uint32_t   port;

     /**
      * Pin to which the output of the bumper sensor is connected to (e.g GPIO9).
      */
     uint16_t	 pin;
} device_data_bumpersensor;

/**
 * Device init function for the bumper sensor.
 *
 * @param[in]	*data	the device data for the device to initialize
 */
void bumpersensor_init(device_data *data);

/**
 * Ask given bumper device if it detected an obstscle.
 *
 * @param[in]	*dev	the bumper device to query
 * @return		1 if obstacle detected, 0 otherwise
 */
int bumpersensor_obstacle_detected(device *dev);

/**
 * Check multible bumber sensors for their current state.
 *
 * @param[in]	devices		arry of bumper devices to check
 * @param[out]	values		value for each device (1 = obstacle, 0 = no obstacle)
 * @param[in]	num_devices	number of devices in array, as well as size of values array
 */
void bumpersensor_obstacle_detected_multi(device_ref devices[], int values[], int num_devices);

#endif

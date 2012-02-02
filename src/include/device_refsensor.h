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

#ifndef __DEVICE_REFSENSOR_H_
#define __DEVICE_REFSENSOR_H_

#include <stdint.h>

#include "device.h"

/**
 * The max value to which to count when measuring the refsensor.
 * This value is mainly to make sure, that the measurement loop
 * surely terminaters.
 */
#define	REFSENSOR_MAX_VALUE		20000

/**
 * Initialization data for a reflection sensor.
 */
typedef struct {
     /**
      * The port on which the reflection sensor is connected. E.g. GPIOB.
      */
     uint32_t   port;

     /**
      * The pin on which the reflection sensor is connected. E.g. GPIO11.
      */
     uint16_t	 pin;
} device_data_refsensor;

/**
 * Device init function for the reflection sensor.
 *
 * @param[in]	*data	the device data for the device to initialize
 */
void refsensor_init(device_data *data);

/**
 * Starte charging of the capacitor of a given reflection sensor.
 *
 * @param[in]	*dev	the device to work on
 */
void refsensor_start_charge(device *dev);

/**
 * Starte charging of the capacitor of a given array of reflection sensors.
 *
 * @param[in]	devices		the arry of references to multible reflection sensors
 * @param[in]	num_devices	number of device references in the array
 */
void refsensor_start_charge_multi(device_ref devices[], int num_devices);

/**
 * Measure the reflection for a given device.
 *
 * @param[in]	*dev	the device to work on
 */
int refsensor_measure(device *dev);

/**
 * Measure the reflection for a given array of reflection sensors.
 *
 * @param[in]	devices		the arry of references to multible reflection sensors
 * @param[out]	values		the values measured for the given sensors
 * @param[in]	num_devices	number of device referencesi and values in the arrays
 */
void refsensor_measure_multi(device_ref devices[], int values[], int num_devices);

#endif

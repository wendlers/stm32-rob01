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

#ifndef __DEVICE_DCMOTOR_H_
#define __DEVICE_DCMOTOR_H_

#include <stdint.h>

#include "device.h"

/**
 * Initialization data for a DC motor connected through a typical H-Bridge.
 */
typedef struct {
     /**
      * Port 1 for motor control. E.g. GPIOC.
      */
     uint32_t   port_ctrl1;

     /**
      * Pin 1 for motor control. E.g. GPIO4.
      */
     uint16_t	 pin_ctrl1;

     /**
      * Port 2 for motor control. E.g. GPIOC.
      */
     uint32_t   port_ctrl2;

     /**
      * Pin 2 for motor control. E.g. GPIO5.
      */
     uint16_t	 pin_ctrl2;

     /**
      * Port on which PWM for speed control is connected to H-Bridge. E.g. GPIOC.
      */
     uint32_t   port_pwm;

     /**
      * Pin on which PWM for speed control is connected to H-Bridge. E.g. GPIO6.
      */
     uint16_t	 pin_pwm;
} device_data_dcmotor;

/**
 * Device init function for the DC motor.
 * Note: currently the PWM is not supported.
 * PWM is always set to high (max speed)
 * on initialization.
 *
 * @param[in]	*data	the device data for the device to initialize
 */
void dcmotor_init(device_data *data);

/**
 * Make the DC motor turn clock-wise.
 *
 * @param[in]	*dev	device representing the DC motor
 */
void dcmotor_cw(device *dev);

/**
 * Make the DC motor turn counter-clock-wise.
 *
 * @param[in]	*dev	device representing the DC motor
 */
void dcmotor_ccw(device *dev);

/**
 * Make the DC motor break (by shortening the moters power supply).
 *
 * @param[in]	*dev	device representing the DC motor
 */
void dcmotor_break(device *dev);

/**
 * Make the DC motor float.
 *
 * @param[in]	*dev	device representing the DC motor
 */
void dcmotor_float(device *dev);

#endif

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

#ifndef __DEVICE_H_
#define __DEVICE_H_

#include <stdint.h>

#include "device.h"

/**
 * Stub type for device data. A concrte device has to define
 * it's particular data.
 */
typedef struct {
} device_data;

/**
 * Defines a device.
 */
typedef struct {
     /**
      * Unique device ID
      */
     const char 		*id;

     /**
      * The RCC register to enable fot this device (e.g. RCC_APB2ENR).
      * The clock for this reg. will be enabled automitically on board
      * initialization.
      */
     volatile uint32_t 	*rcc_reg;

     /**
      * The port to enable on the given register (rcc_reg).
      * This enables the clock for the port on board initialization.
      * E.g.  RCC_APB2ENR_IOPAEN.
      */
     uint16_t  		 rcc_en;

     /**
      * Device specific initialization data passed to the device specific
      * init function.
      */
     device_data 		*data;

     /**
      * Function callback to the device specific init function.
      */
     void (*init)(device_data *data);
} device;

/**
 * Device reference.
 */
typedef device* device_ref;

#endif

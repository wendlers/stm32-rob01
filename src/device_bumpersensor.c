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

#include <libopencm3/stm32/f1/gpio.h>

#include "device_bumpersensor.h"

void bumpersensor_init(device_data *data)
{
     device_data_bumpersensor *dd = (device_data_bumpersensor *)data;

     gpio_set_mode(dd->port, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, dd->pin);
}

int bumpersensor_obstacle_detected(device *dev)
{
     device_data_bumpersensor *dd = (device_data_bumpersensor *)dev->data;

     return ((GPIO_IDR(dd->port) & dd->pin) == 0);
}

void bumpersensor_obstacle_detected_multi(device_ref devices[], int values[], int num_devices)
{
     int i;

     for(i = 0; i < num_devices; i++) {
          values[i] = bumpersensor_obstacle_detected(devices[i]);
     }
}

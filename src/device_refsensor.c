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

#include "device_refsensor.h"

void refsensor_init(device_data *data)
{
     device_data_refsensor *dd = (device_data_refsensor *)data;

     gpio_set_mode(dd->port, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, dd->pin);
     gpio_set(dd->port, dd->pin);
}

void refsensor_start_charge(device *dev)
{
     device_data_refsensor *dd = (device_data_refsensor *)dev->data;

     gpio_set_mode(dd->port, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, dd->pin);
     gpio_set(dd->port, dd->pin);
}

void refsensor_start_charge_multi(device_ref devices[], int num_devices)
{

     int i;

     for(i = 0; i < num_devices; i++) {
          refsensor_start_charge(devices[i]);
     }
}

int refsensor_measure(device *dev)
{

     int value = 0;
     device_data_refsensor *dd = (device_data_refsensor *)dev->data;

     gpio_set_mode(dd->port, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, dd->pin);

     while(!((GPIO_IDR(dd->port) & dd->pin) == 0) && value < REFSENSOR_MAX_VALUE) {
          value++;
     }

     return value;
}

void refsensor_measure_multi(device_ref devices[], int values[], int num_devices)
{

     int i;
     int num_measured;
     device_data_refsensor *dd;

     // start discharge
     for(i = 0; i < num_devices; i++) {
          dd = (device_data_refsensor *)devices[i]->data;
          gpio_set_mode(dd->port, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, dd->pin);
          values[i] = 0;
     }

     // measure discharging time
     do {
          num_measured = 0;

          for(i = 0; i < num_devices; i++) {
               dd = (device_data_refsensor *)devices[i]->data;

               if(!((GPIO_IDR(dd->port) & dd->pin) == 0) && values[i] < REFSENSOR_MAX_VALUE) {
                    values[i]++;
                    num_measured++;
               }
          }
     } while(num_measured > 0);
}


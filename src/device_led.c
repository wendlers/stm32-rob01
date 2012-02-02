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

#include "device_led.h"

void led_init(device_data *data)
{
     device_data_led *dd = (device_data_led *)data;

     gpio_set_mode(dd->port, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, dd->pin);
}

void led_on(device *dev)
{
     device_data_led *dd = (device_data_led *)dev->data;

     gpio_set(dd->port, dd->pin);
}

void led_off(device *dev)
{
     device_data_led *dd = (device_data_led *)dev->data;
     gpio_clear(dd->port, dd->pin);
}

void led_toggle(device *dev)
{
     device_data_led *dd = (device_data_led *)dev->data;

     gpio_toggle(dd->port, dd->pin);
}

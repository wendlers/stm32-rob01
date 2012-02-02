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

#include "device_button.h"

void button_init(device_data *data)
{
     device_data_button *dd = (device_data_button *)data;

     gpio_set_mode(dd->port, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, dd->pin);
}

void button_init_pullup(device_data *data)
{
     device_data_button *dd = (device_data_button *)data;

     gpio_set_mode(dd->port, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, dd->pin);
     gpio_set(dd->port, dd->pin);
}

void button_init_pulldown(device_data *data)
{
     device_data_button *dd = (device_data_button *)data;

     gpio_set_mode(dd->port, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, dd->pin);
     gpio_clear(dd->port, dd->pin);
}

int button_is_pressed(device *dev)
{
     device_data_button *dd = (device_data_button *)dev->data;

     return !((GPIO_IDR(dd->port) & dd->pin) == 0);
}


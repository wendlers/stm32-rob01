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

#ifndef __DEVICE_SERIAL_H_
#define __DEVICE_SERIAL_H_

#include <stdint.h>

#include "device.h"

/**
 * Initialization data for a serial (USART)  port.
 */
typedef struct {
     /**
      * The USART to use for this serial device. E.g. USART1.
      */
     uint32_t	usart;

     /**
      * The port to use for this serial device. E.g. GPIOA.
      */
     uint32_t  port;

     /**
      * The pin which is used for TX. E.g. GPIO_USART1_TX.
      */
     uint16_t	pin_tx;

     /**
      * The pin which is used for RX. E.g. GPIO_USART1_RX.
      */
     uint16_t	pin_rx;

     /**
      * The baud rate to use for communication. E.g. 38400.
      */
     uint32_t	baud;

     /**
      * Enable the IRQ for this serial device.
      * 1 = enable, 0 = disable. If enabled, the IRQ handler
      * function has to be defiend somewhere in the code
      * (usart1_isr, usart2_isr, ...).
      */
     uint32_t	irq_en;
} device_data_serial;

/**
 * Device init function for the serial port.
 *
 * @param[in]	*data	the device data for the device to initialize
 */
void serial_init(device_data *data);

/**
 * Send data non-blocking.
 *
 * @param	*dev	the device to work on
 * @param	data	data to send to the serial port
 */
void serial_send(device *dev, uint16_t data);

/**
 * Receive data non-blocking.
 *
 * @param	*dev	the device to work on
 * @return	data received, may be -1 if no data was available
 */
uint16_t serial_recv(device *dev);

/**
 * Send data blocking.
 *
 * @param	*dev	the device to work on
 * @param	data	data to send to the serial port
 */
void serial_send_blocking(device *dev, uint16_t data);

/**
 * Receive data blocking.
 *
 * @param	*dev	the device to work on
 * @return	data received
 */
uint16_t serial_recv_blocking(device *dev);

#endif

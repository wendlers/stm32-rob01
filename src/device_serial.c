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
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/f1/nvic.h>

#include "board.h"
#include "device_serial.h"

void serial_init(device_data *data)
{
     device_data_serial *dd = (device_data_serial *)data;

     gpio_set_mode(dd->port, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, dd->pin_tx);
     gpio_set_mode(dd->port, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, dd->pin_rx);

     USART_BRR(dd->usart) = (uint16_t)((BOARD_SPEED_IN_MHZ  * 1000000 << 4) / (dd->baud * 16));

     usart_set_databits(dd->usart, 8);
     usart_set_stopbits(dd->usart, USART_STOPBITS_1);
     usart_set_mode(dd->usart, USART_MODE_TX_RX);
     usart_set_parity(dd->usart, USART_PARITY_NONE);
     usart_set_flow_control(dd->usart, USART_FLOWCONTROL_NONE);

     if(dd->irq_en) {
          USART_CR1(dd->usart) |= dd->irq_en;

          switch(dd->usart) {
          case USART1:
               nvic_enable_irq(NVIC_USART1_IRQ);
               break;
          case USART2:
               nvic_enable_irq(NVIC_USART2_IRQ);
               break;
          case USART3:
               nvic_enable_irq(NVIC_USART3_IRQ);
               break;
               /*
               			case USART4:
               				nvic_enable_irq(NVIC_USART4_IRQ);
               			break;
               			case USART5:
               				nvic_enable_irq(NVIC_USART5_IRQ);
               			break;
               */
          }
     }

     usart_enable(dd->usart);
}

void serial_send(device *dev, uint16_t data)
{
     device_data_serial *dd = (device_data_serial *)dev->data;
     usart_send(dd->usart, data);
}

uint16_t serial_recv(device *dev)
{
     device_data_serial *dd = (device_data_serial *)dev->data;
     return usart_recv(dd->usart);
}

void serial_send_blocking(device *dev, uint16_t data)
{
     device_data_serial *dd = (device_data_serial *)dev->data;
     usart_send_blocking(dd->usart,data);
}

uint16_t serial_recv_blocking(device *dev)
{
     device_data_serial *dd = (device_data_serial *)dev->data;
     return usart_recv_blocking(dd->usart);
}

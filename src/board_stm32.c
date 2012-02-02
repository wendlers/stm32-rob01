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
#include <libopencm3/stm32/f1/rcc.h>
#include <libopencm3/stm32/usart.h>

#include "board.h"
#include "device_serial.h"
#include "device_led.h"
#include "device_dcmotor.h"
#include "device_bumpersensor.h"
#include "device_refsensor.h"
#include "device_button.h"

// Define serial port
device_data_serial serial_data = {
     .usart	= USART1,
     .port	= GPIOA,
     .pin_tx	= GPIO_USART1_TX,
     .pin_rx	= GPIO_USART1_RX,
     .baud	= 38400,
     .irq_en	= USART_CR1_RXNEIE,
};

device serial = {
     .id 		= "SERIAL",
     .rcc_reg 	= &RCC_APB2ENR,
     .rcc_en  	= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_USART1EN,
     .data	= (device_data *)&serial_data,
     .init 	= serial_init,
};

// Define starus LEDs
device_data_led status_led1_data = {
     .port 	= GPIOC,
     .pin 	= GPIO8,
};

device_data_led status_led2_data = {
     .port 	= GPIOC,
     .pin 	= GPIO9,
};

device status_led1 = {
     .id 		= "LED1",
     .rcc_reg 	= &RCC_APB2ENR,
     .rcc_en  	= RCC_APB2ENR_IOPCEN,
     .data	= (device_data *)&status_led1_data,
     .init 	= led_init,
};

device status_led2 = {
     .id 		= "LED2",
     .rcc_reg 	= &RCC_APB2ENR,
     .rcc_en  	= RCC_APB2ENR_IOPCEN,
     .data	= (device_data *)&status_led2_data,
     .init 	= led_init,
};

// Define the DC motors
device_data_dcmotor dcmotor_left_data = {
     .port_ctrl1	= GPIOB,	// AIN1 @ PA5
     .pin_ctrl1	= GPIO5,
     .port_ctrl2	= GPIOC,	// AIN2 @ PC12
     .pin_ctrl2	= GPIO12,
     .port_pwm		= GPIOC,	// PWMA @ PC11
     .pin_pwm		= GPIO11,
};

device_data_dcmotor dcmotor_right_data = {
     .port_ctrl1	= GPIOB,	// BIN1 @ PB7
     .pin_ctrl1	= GPIO7,
     .port_ctrl2	= GPIOB,	// BIN2 @ PB8
     .pin_ctrl2	= GPIO8,
     .port_pwm		= GPIOB,	// PWMB @ PB9
     .pin_pwm		= GPIO9,
};

device dcmotor_left = {
     .id 		= "DC_MOT_LEFT",
     .rcc_reg 	= &RCC_APB2ENR,
     .rcc_en  	= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPCEN,
     .data	= (device_data *)&dcmotor_left_data,
     .init 	= dcmotor_init,
};

device dcmotor_right = {
     .id 		= "DC_MOT_RIGHT",
     .rcc_reg 	= &RCC_APB2ENR,
     .rcc_en  	= RCC_APB2ENR_IOPBEN,
     .data	= (device_data *)&dcmotor_right_data,
     .init 	= dcmotor_init,
};

// Define the bumper sensors
device_data_bumpersensor bumpersensor_left_data = {
     .port 	= GPIOA,		// Left bumper @ PA1
     .pin 	= GPIO1,
};

device_data_bumpersensor bumpersensor_right_data = {
     .port 	= GPIOA,		// Right bumper @ PA2
     .pin 	= GPIO2,
};

device bumpersensor_left = {
     .id 		= "BUMPER_LEFT",
     .rcc_reg 	= &RCC_APB2ENR,
     .rcc_en  	= RCC_APB2ENR_IOPAEN,
     .data	= (device_data *)&bumpersensor_left_data,
     .init 	= bumpersensor_init,
};

device bumpersensor_right = {
     .id 		= "BUMPER_RIGHT",
     .rcc_reg 	= &RCC_APB2ENR,
     .rcc_en  	= RCC_APB2ENR_IOPAEN,
     .data	= (device_data *)&bumpersensor_right_data,
     .init 	= bumpersensor_init,
};

// Define the reflection sensors
device_data_refsensor refsensor_left_data = {
     .port 	= GPIOA,		// Left refsensor @ PA3
     .pin 	= GPIO3,
};

device_data_refsensor refsensor_right_data = {
     .port 	= GPIOA,		// Right refsensor @ PA4
     .pin 	= GPIO4,
};

device refsensor_left = {
     .id 		= "REF_LEFT",
     .rcc_reg 	= &RCC_APB2ENR,
     .rcc_en  	= RCC_APB2ENR_IOPAEN,
     .data	= (device_data *)&refsensor_left_data,
     .init 	= refsensor_init,
};

device refsensor_right = {
     .id 		= "REF_RIGHT",
     .rcc_reg 	= &RCC_APB2ENR,
     .rcc_en  	= RCC_APB2ENR_IOPAEN,
     .data	= (device_data *)&refsensor_right_data,
     .init 	= refsensor_init,
};

// Define the user button
device_data_button button_user1_data = {
     .port 	= GPIOA,		// user button 1 @ PA0
     .pin 	= GPIO0,
};

device button_user1 = {
     .id 		= "BUTT_USER1",
     .rcc_reg 	= &RCC_APB2ENR,
     .rcc_en  	= RCC_APB2ENR_IOPAEN,
     .data	= (device_data *)&button_user1_data,
     .init 	= button_init,
};

// All my board devcies
board_devices my_devices = {
     .size = 10,
     .device_array = {
          &serial,
          &status_led1,
          &status_led2,
          &dcmotor_left,
          &dcmotor_right,
          &bumpersensor_left,
          &bumpersensor_right,
          &refsensor_left,
          &refsensor_right,
          &button_user1,
     },
};

void board_init_rcc(board_devices *devs)
{
     int i;

     uint32_t apb1en = 0;
     uint32_t apb2en = 0;

     for(i = 0; i < devs->size; i++) {
          if(devs->device_array[i]->rcc_reg == &RCC_APB1ENR) {
               apb1en |= devs->device_array[i]->rcc_en;
          } else if(devs->device_array[i]->rcc_reg == &RCC_APB2ENR) {
               apb2en |= devs->device_array[i]->rcc_en;
          }
     }

     rcc_clock_setup_in_hse_8mhz_out_24mhz();
     // rcc_clock_setup_in_hse_8mhz_out_72mhz();

     if(apb1en != 0) {
          rcc_peripheral_enable_clock(&RCC_APB1ENR, apb1en);
     }
     if(apb2en != 0) {
          rcc_peripheral_enable_clock(&RCC_APB2ENR, apb2en);
     }
}

void board_init_devices(board_devices *devs)
{
     int i;

     for(i = 0; i < devs->size; i++) {
          (devs->device_array[i]->init)(devs->device_array[i]->data);
     }
}

void board_init()
{
     board_init_rcc(&my_devices);
     board_init_devices(&my_devices);
}

void board_init_systick(uint8_t clocksource, uint32_t reload)
{

     systick_set_clocksource(clocksource);
     systick_set_reload(reload);
     systick_interrupt_enable();
     systick_counter_enable();
}

void board_init_rtc()
{
     rtc_auto_awake(LSE, 0x7fff);
}

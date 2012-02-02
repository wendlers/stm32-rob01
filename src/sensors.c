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

#include <libopencm3/stm32/f1/rcc.h>
#include <libopencm3/stm32/f1/rtc.h>

#include "board.h"
#include "sensors.h"
#include "device_button.h"
#include "device_refsensor.h"
#include "device_bumpersensor.h"

// holds all currently measured sensor values
sensor_values sensors = {
     .bumper_left = {
          .hit = 0,
     },
     .bumper_right = {
          .hit = 0,
     },
     .odometer_left = {
          .count = 0,
          .ppm	 = 0,
          .lp    = 0,
     },
     .odometer_right = {
          .count = 0,
          .ppm	 = 0,
          .lp    = 0,
     },
     .inputbutton = {
          .state = 0,
     },
};

// sensors could be enabled or disabled
sensors_enabled sensors_en = {
     .bumper = 1,
     .odometer = 1,
     .inputbutton = 1,
};

void sensors_aquire_bumper()
{
     if(!sensors_en.bumper) return;

     static device_ref bumpersensors[] = {
          &bumpersensor_left,
          &bumpersensor_right,
     };

     static int bumpersensor_values[2];
     static int bumpersensor_values_prev[2];

     int i;

     bumpersensor_obstacle_detected_multi(bumpersensors, bumpersensor_values, 2);

     for(i = 0; i < 2; i++) {
          if(bumpersensor_values[i] != bumpersensor_values_prev[i]) {

               bumpersensor_values_prev[i] = bumpersensor_values[i];

               if(i == 0) {
                    sensors.bumper_left.hit  = bumpersensor_values[i];
               } else {
                    sensors.bumper_right.hit = bumpersensor_values[i];
               }
          }
     }
}

int sensors_check_odo_level(int level, int level_prev)
{
     // count white->black and black->white transitions
     if((level > SENSORS_ODO_MIN_REFL && level_prev < SENSORS_ODO_MIN_REFL) ||
               (level < SENSORS_ODO_MIN_REFL && level_prev > SENSORS_ODO_MIN_REFL)) {
          return 1;
     }

     return 0;
}

void sensors_aquire_odometer()
{
     if(!sensors_en.odometer) return;

     static uint32_t refsensor_ticks = 0;

     static device_ref refsensors[] = {
          &refsensor_left,
          &refsensor_right,
     };

     static int refsensor_values[2];
     static int refsensor_values_prev[2];

     static int lcl = 0;	// last count left
     static int lcr = 0;	// last count right

     switch(refsensor_ticks) {
     case 0:
          refsensor_start_charge_multi(refsensors, 2);
          refsensor_ticks++;
          break;
     case 100:
          refsensor_measure_multi(refsensors, refsensor_values, 2);

          int lp  = rtc_get_counter_val();	// time stamp of last poll

          int dlp_left  = lp - sensors.odometer_left.lp;
          int dlp_right = lp - sensors.odometer_right.lp;

          sensors.odometer_left.count += sensors_check_odo_level(refsensor_values[0],
                                         refsensor_values_prev[0]);

          refsensor_values_prev[0] = refsensor_values[0];

          sensors.odometer_right.count += sensors_check_odo_level(refsensor_values[1],
                                          refsensor_values_prev[1]);

          refsensor_values_prev[1] = refsensor_values[1];

          // calc ppm (if time advanced)
          if(dlp_left >= 1) {
               sensors.odometer_left.ppm = (60 / dlp_left) * (sensors.odometer_left.count - lcl);
               sensors.odometer_left.lp  = lp;
               lcl = sensors.odometer_left.count;
          }
          if(dlp_right >= 1) {
               sensors.odometer_right.ppm = (60 / dlp_right) * (sensors.odometer_right.count - lcr);
               sensors.odometer_right.lp = lp;
               lcr = sensors.odometer_right.count;
          }

          refsensor_ticks = 0;

          break;
     default:
          refsensor_ticks++;
     }
}

void sensors_aquire_inputbutton()
{
     if(!sensors_en.inputbutton) return;

     static int button_pressed = 0;

     if(!button_pressed && button_is_pressed(&button_user1)) {
          button_pressed = 1;
     } else if(button_pressed && !button_is_pressed(&button_user1)) {
          button_pressed = 0;
     }

     sensors.inputbutton.state = button_pressed;
}

void sensors_aquire()
{
     sensors_aquire_bumper();
     sensors_aquire_odometer();
     sensors_aquire_inputbutton();
}

void sensor_reset_bumper()
{
     sensors.bumper_left.hit  = 0;
     sensors.bumper_right.hit = 0;
}

void sensor_reset_odometer()
{
     sensors.odometer_left.count  	= 0;
     sensors.odometer_left.ppm  	= 0;
     sensors.odometer_left.lp  		= 0;

     sensors.odometer_right.count 	= 0;
     sensors.odometer_right.ppm 	= 0;
     sensors.odometer_right.lp 		= 0;
}

void sensor_reset_inputbutton()
{
     sensors.inputbutton.state = 0;
}

void sys_tick_handler()
{
     sensors_aquire();
}

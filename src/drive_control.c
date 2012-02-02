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

#include "sensors.h"
#include "actors.h"
#include "drive_control.h"

#include "shell.h"

/**
 * If the PPM (pulses-per-minute) of the wheels goes below this
 * threshold, the wheel is considered to be stuck.
 */
#define DRIVE_CTRL_PPM_LOW			 	 190

/**
 * If the PPM (pulses-per-minute) of the wheels goes above this
 * threshold, the whill is considered to have no traction anay more.
 */
#define DRIVE_CTRL_PPM_HIGH				 320

/**
 * Sensors detected NO obstacle at all.
 */
#define DRIVE_CTRL_OBST_NONE			 0

/**
 * Sensors detected obstacle on FRONT LEFT.
 */
#define DRIVE_CTRL_OBST_FRONT_LEFT		 1

/**
 * Sensors detected obstacle on FRONT RIGHT.
 */
#define DRIVE_CTRL_OBST_FRONT_RIGHT	 	 2

/**
 * Sensors detected obstacle on FRONT LEFT + RIGHT.
 */
#define DRIVE_CTRL_OBST_FRONT_BOTH		 3

/**
 * Sensors detected obstacle on BACK LEFT.
 */
#define DRIVE_CTRL_OBST_BACK_LEFT		 4

/**
 * Sensors detected obstacle on BACK RIGHT.
 */
#define DRIVE_CTRL_OBST_BACK_RIGHT		 8

/**
 * Sensors detected obstacle on BACK LEFT+RIGHT.
 */
#define DRIVE_CTRL_OBST_BACK_BOTH		12

/**
 * Sensors detected obstacle on FRONT+BACK LEFT+RIGHT.
 */
#define DRIVE_CTRL_OBST_ALL				15

/**
 * Timestsmp holding the start time for following a path segment.
 */
int fps_start;

/**
 * Check all sensors, and agregate the result. A flag will be returned
 * indicating, where an obstacle was detected.
 *
 * @param[in]	with_bumpers	set to 1 if bumper sensors should be taken into account
 * @return		the obstacle flag, made up from the DRIVE_CTRL_OBST_x constants.
 */
int drive_control_agregate_sensors(int with_bumpers)
{
     int stat = DRIVE_CTRL_OBST_NONE;

     int stuck_left  = 0;
     int stuck_right = 0;

     /**
      * checking the PPM has to bedelayed, since it takes some time
      * after starting moving the vehicle until a valid PPM value is
      * available.
      */
     int now = rtc_get_counter_val();

     if((now - fps_start >= 2) && (
                    sensors.odometer_left.ppm < DRIVE_CTRL_PPM_LOW ||
                    sensors.odometer_left.ppm > DRIVE_CTRL_PPM_HIGH)) {
          stuck_left = 1;
     }
     if((now - fps_start >= 2) && (
                    sensors.odometer_right.ppm < DRIVE_CTRL_PPM_LOW ||
                    sensors.odometer_right.ppm >  DRIVE_CTRL_PPM_HIGH)) {
          stuck_right = 1;
     }

     if(with_bumpers) {
          if(sensors.bumper_left.hit) {
               stat |= DRIVE_CTRL_OBST_FRONT_LEFT;
          }
          if(sensors.bumper_right.hit) {
               stat |= DRIVE_CTRL_OBST_FRONT_RIGHT;
          }
     }

     if(actors.vehicle.direction == VEHICLE_DIR_FORWARD) {
          if(stuck_left) {
               stat |= DRIVE_CTRL_OBST_FRONT_LEFT;
          }
          if(stuck_right) {
               stat |= DRIVE_CTRL_OBST_FRONT_RIGHT;
          }
     } else if(actors.vehicle.direction == VEHICLE_DIR_BACKWARD) {
          if(stuck_left) {
               stat |= DRIVE_CTRL_OBST_BACK_LEFT;
          }
          if(stuck_right) {
               stat |= DRIVE_CTRL_OBST_BACK_RIGHT;
          }
     } else if(actors.vehicle.direction == VEHICLE_DIR_FORWARD_LEFT && stuck_right) {
          stat |= DRIVE_CTRL_OBST_FRONT_LEFT;
     } else if(actors.vehicle.direction == VEHICLE_DIR_FORWARD_RIGHT && stuck_left) {
          stat |= DRIVE_CTRL_OBST_FRONT_RIGHT;
     } else if(actors.vehicle.direction == VEHICLE_DIR_BACKWARD_LEFT && stuck_right) {
          stat |= DRIVE_CTRL_OBST_BACK_LEFT;
     } else if(actors.vehicle.direction == VEHICLE_DIR_BACKWARD_RIGHT && stuck_left) {
          stat |= DRIVE_CTRL_OBST_BACK_RIGHT;
     }

     return stat;
}

/**
 * This function checks if an "emergency condition" is reached while drigin the vehicle.
 * It will return 1 if the bumpers detected an obstacle, or if the wheels are considered
 * to be stuck. Checking the bumpers could be disabled by setting "with_bumpers" to 0.
 *
 * @param[in]	with_bumpers	check bumpers = 1, ignore bumpers = 0
 * @return 	1 on emergency conditon, 0 otherwise
 */
int drive_control_ems(int with_bumpers)
{
     int obst = drive_control_agregate_sensors(with_bumpers);

     if(obst != DRIVE_CTRL_OBST_NONE) {
          actor_vehicle_break();
          shell_print("obstacle detected: ");
          shell_println_int(obst);
          return 1;
     }

     return 0;
}

/**
 * Used in auto mode to decide which path segement to follow next.
 *
 * param[inioout]	*ps
 */
void drive_control_find_path(path_segment *ps)
{
     ps->count = 2;

     int stat = drive_control_agregate_sensors(1);

     switch(stat) {
     case DRIVE_CTRL_OBST_ALL:
//          shell_println("Completely stuck - giving up");
          ps->direction = VEHICLE_DIR_NONE;
          break;
     case DRIVE_CTRL_OBST_FRONT_BOTH:
//          shell_println("Obstacle left+right front");
          ps->direction = VEHICLE_DIR_BACKWARD;
          ps->count = 3;
          break;
     case DRIVE_CTRL_OBST_BACK_BOTH:
//          shell_println("Obstacle left+right back");
          ps->direction = VEHICLE_DIR_FORWARD;
          ps->count = 0;
          break;
     case DRIVE_CTRL_OBST_FRONT_LEFT:
//          shell_println("Obstacle left front");
          ps->direction = VEHICLE_DIR_TURN_RIGHT;
          break;
     case DRIVE_CTRL_OBST_FRONT_RIGHT:
//          shell_println("Obstacle right front");
          ps->direction = VEHICLE_DIR_TURN_LEFT;
          break;
     case DRIVE_CTRL_OBST_BACK_LEFT:
//          shell_println("Obstacle left back");
          ps->direction = VEHICLE_DIR_TURN_RIGHT;
          break;
     case DRIVE_CTRL_OBST_BACK_RIGHT:
//          shell_println("Obstacle right back");
          ps->direction = VEHICLE_DIR_TURN_LEFT;
          break;
     case DRIVE_CTRL_OBST_NONE:
//          shell_println("Obstacle none");
          ps->direction = VEHICLE_DIR_FORWARD;
          ps->count = 0;
          break;
     };
}

void drive_control_follow_path(path_segment *ps)
{
     fps_start = rtc_get_counter_val();

     int b = 0;
     int l = 0;
     int r = 0;

     int ol = sensors.odometer_left.count  + ps->count;
     int or = sensors.odometer_right.count + ps->count;

     switch(ps->direction) {
     case VEHICLE_DIR_NONE:
          actor_vehicle_break();
          break;
     case VEHICLE_DIR_FORWARD:
          actor_vehicle_forward();
          b = l = r = 1;
          break;
     case VEHICLE_DIR_BACKWARD:
          actor_vehicle_backward();
          l = r = 1;
          break;
     case VEHICLE_DIR_FORWARD_LEFT:
          actor_vehicle_forward_left();
          r = 1;
          break;
     case VEHICLE_DIR_FORWARD_RIGHT:
          actor_vehicle_forward_right();
          l = 1;
          break;
     case VEHICLE_DIR_BACKWARD_LEFT:
          actor_vehicle_backward_left();
          r = 1;
          break;
     case VEHICLE_DIR_BACKWARD_RIGHT:
          actor_vehicle_backward_right();
          l = 1;
          break;
     case VEHICLE_DIR_TURN_LEFT:
          actor_vehicle_turn_left();
          l = r = 1;
          break;
     case VEHICLE_DIR_TURN_RIGHT:
          actor_vehicle_turn_right();
          l = r = 1;
          break;
     }

     if(ps->count > 0) {
          while(!drive_control_ems(b) && actors.vehicle.direction != VEHICLE_DIR_NONE) {
               if((l && sensors.odometer_left.count  >= ol) ||
                         (r && sensors.odometer_right.count >= or)) {
                    actor_vehicle_break();
               }
          }
     }
}

void drive_control_auto()
{
     static path_segment ps = {
          .count = 0,
          .direction = VEHICLE_DIR_NONE,
     };

     // find path
     drive_control_find_path(&ps);

     // adapt move
     drive_control_follow_path(&ps);
}


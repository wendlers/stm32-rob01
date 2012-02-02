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

#ifndef __SENSORS_H_
#define __SENSORS_H_

#define SENSORS_ODO_MIN_REFL		400

/**
 * Sensor values aquired form a bumper sensor.
 */
typedef struct {
     /**
      * Indicates if the sensor was hit (by detecting an obstacle).
      * 1 = hit, 0 = no hit
      */
     int	hit;
} sensor_value_bumper;

/**
 * Sensor values aquired from an odometric sensor.
 */
typedef struct {
     /**
      * Total count of black/white changes on this sensor (since last reset)
      */
     int	count;

     /**
      * The puls changes per minute
      */
     int	ppm;

     /**
      * The timestamp when this sensor was last checked and the ppm was calculated
      */
     int	lp;
} sensor_value_odometer;

/**
 * Sensor values aquired form an input button.
 */
typedef struct {
     /**
      * The button state
      * 1 = pressed, 0 = released
      */
     int	state;
} sensor_value_inputbutton;

/**
 * All sensor values available in this system.
 */
typedef struct {
     /**
      * The left bumper sensor
      */
     sensor_value_bumper		bumper_left;

     /**
      * The right bumper sensor
      */
     sensor_value_bumper		bumper_right;

     /**
      * The left odometer
      */
     sensor_value_odometer	odometer_left;

     /**
      * The right odometer
      */
     sensor_value_odometer	odometer_right;

     /**
      * The input button
      */
     sensor_value_inputbutton	inputbutton;
} sensor_values;

/**
 * Struct do enable/disable measurement of defined sensor-blocks.
 */
typedef struct {
     /**
      * Enable measurement of bumper sensors (left+right).
      * 1 = enable, 0 = disable
      */
     int bumper;

     /**
      * Enable measurement of odometric sensors (left+right).
      * 1 = enable, 0 = disable
      */
     int odometer;

     /**
      * Enable measurement of button.
      * 1 = enable, 0 = disable
      */
     int inputbutton;
} sensors_enabled;

/**
 * Global struct for enabling/disabling sensor-blocks.
 */
extern sensors_enabled sensors_en;

/**
 * Global struct holding all available sensor values of the system.
 */
extern sensor_values sensors;

/**
 * Aquire the values for all sensors which are enables (see {@link sensors_en}).
 * This operation is normaly called within the systick handler or somthing similar which
 * is called regularly.
 */
void sensors_aquire();

/**
 * Reset (to 0) the sensor values for the bumpers (left+right).
 */
void sensor_reset_bumper();

/**
 * Reset (to 0) the sensor values for the odometers (left+right).
 */
void sensor_reset_odometer();

/**
 * Reset (to 0) the sensor values for the button.
 */
void sensor_reset_inputbutton();

#endif

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

#ifndef __ACTORS_H_
#define __ACTORS_H_

/**
 * Vehicle is not moving at all.
 */
#define VEHICLE_DIR_NONE			0

/**
 * Vehicle is moving forward.
 */
#define VEHICLE_DIR_FORWARD		1

/**
 * Vehicle is moving backward.
 */
#define VEHICLE_DIR_BACKWARD		2

/**
 * Vehicle is moving forward-left.
 */
#define VEHICLE_DIR_FORWARD_LEFT	3

/**
 * Vehicle is moving forward-right.
 */
#define VEHICLE_DIR_FORWARD_RIGHT	4

/**
 * Vehicle is moving backward-left.
 */
#define VEHICLE_DIR_BACKWARD_LEFT	5

/**
 * Vehicle is moving backward-right.
 */
#define VEHICLE_DIR_BACKWARD_RIGHT	6

/**
 * Vehicle is turning left.
 */
#define VEHICLE_DIR_TURN_LEFT		7

/**
 * Vehicle is turning right.
 */
#define VEHICLE_DIR_TURN_RIGHT	8

/**
 * State of the actor "vehicle".
 */
typedef struct {
     /**
      * The direction in which the vehicle is currently moving (one of VEHICLE_DIR_xx).
      */
     int	direction;
} actor_state_vehicle;

/**
 * The states of all actors.
 */
typedef struct {
     /**
      * The state of the actor "vehicle"
      */
     actor_state_vehicle	vehicle;
} actor_states;

/**
 * Global variable holding the state of all actors.
 */
extern actor_states actors;

/**
 * Tell actor "vehicle" to break (stop).
 */
void actor_vehicle_break();

/**
 * Tell actor "vehicle" to move forward.
 */
void actor_vehicle_forward();

/**
 * Tell actor "vehicle" to move forward-left.
 */
void actor_vehicle_forward_left();

/**
 * Tell actor "vehicle" to move forward-right.
 */
void actor_vehicle_forward_right();

/**
 * Tell actor "vehicle" to move backward.
 */
void actor_vehicle_backward();

/**
 * Tell actor "vehicle" to move backward-left.
 */
void actor_vehicle_backward_left();

/**
 * Tell actor "vehicle" to move backward-right.
 */
void actor_vehicle_backward_right();

/**
 * Tell actor "vehicle" to turn left.
 */
void actor_vehicle_turn_left();

/**
 * Tell actor "vehicle" to turn right.
 */
void actor_vehicle_turn_right();

#endif

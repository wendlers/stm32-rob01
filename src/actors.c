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

#include "actors.h"
#include "board.h"
#include "device_dcmotor.h"

actor_states actors = {
     .vehicle = {
          .direction = VEHICLE_DIR_NONE,
     },
};

void actor_vehicle_break()
{
     actors.vehicle.direction = VEHICLE_DIR_NONE;

     dcmotor_break(&dcmotor_left);
     dcmotor_break(&dcmotor_right);
}

void actor_vehicle_forward()
{
     actors.vehicle.direction = VEHICLE_DIR_FORWARD;

     dcmotor_ccw(&dcmotor_left);
     dcmotor_ccw(&dcmotor_right);
}

void actor_vehicle_forward_left()
{
     actors.vehicle.direction = VEHICLE_DIR_FORWARD_LEFT;

     dcmotor_break(&dcmotor_left);
     dcmotor_ccw(&dcmotor_right);
}

void actor_vehicle_forward_right()
{
     actors.vehicle.direction = VEHICLE_DIR_FORWARD_RIGHT;

     dcmotor_ccw(&dcmotor_left);
     dcmotor_break(&dcmotor_right);
}

void actor_vehicle_backward()
{
     actors.vehicle.direction = VEHICLE_DIR_BACKWARD;

     dcmotor_cw(&dcmotor_left);
     dcmotor_cw(&dcmotor_right);
}

void actor_vehicle_backward_left()
{
     actors.vehicle.direction = VEHICLE_DIR_BACKWARD_LEFT;

     dcmotor_break(&dcmotor_left);
     dcmotor_cw(&dcmotor_right);
}

void actor_vehicle_backward_right()
{
     actors.vehicle.direction = VEHICLE_DIR_BACKWARD_RIGHT;

     dcmotor_cw(&dcmotor_left);
     dcmotor_break(&dcmotor_right);
}

void actor_vehicle_turn_left()
{
     actors.vehicle.direction = VEHICLE_DIR_TURN_LEFT;

     dcmotor_cw(&dcmotor_left);
     dcmotor_ccw(&dcmotor_right);
}

void actor_vehicle_turn_right()
{
     actors.vehicle.direction = VEHICLE_DIR_TURN_RIGHT;

     dcmotor_ccw(&dcmotor_left);
     dcmotor_cw(&dcmotor_right);
}


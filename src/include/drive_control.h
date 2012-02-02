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

#ifndef __DRIVECONTROL_H_
#define __DRIVECONTROL_H_

/**
 * Definition of a path segment. A path segment describes a single movment action the
 * vehicle is able to perform. It consits of a direction, the number of wheel turns and
 * an optional reason which states whay this move was choosen.
 */
typedef struct {
     /**
      * The direction as defined by the constants VEHICLE_DIR_n in {@link actors.h}.
      */
     int	direction;

     /**
      * The number of wheel turn counts as measured by the odometric sensors.
      */
     int	count;

     /**
      * Reason why this move was choosen.
      */
     int	reason;
} path_segment;

/**
 * Perform the auto control. This method should be called in the applictions main loop.
 * It then tries to auto navigate the vhicle by using the sensors input. Thus, the sensors
 * must be enabled for auto control to work properly.
 */
void drive_control_auto();

/**
 * Make the vehicle follow the given path segment. There is no guarnty, thet the vehicle was
 * able to follow the given path. It may stop following the path, if an obstacle was detected
 * by the bumper sensors, or if the vehicle is considered to be stuck.
 *
 * @param[in]	*ps	the path segment to execute
 */
void drive_control_follow_path(path_segment *ps);

#endif

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

#ifndef __SHELLM2M_H_
#define __SHELLM2M_H_

#include "shell.h"

/**
 * Shell command which prints out command help.
 *
 * @param[in]	*args	the arguments form the commandline
 * @return			result of the command, one of SHELL_PROCESS_xx
 */
int shell_cmd_help(shell_cmd_args *args);

/**
 * Shell command which prints out firmware version.
 *
 * @param[in]	*args	the arguments form the commandline
 * @return			result of the command, one of SHELL_PROCESS_xx
 */
int shell_cmd_version(shell_cmd_args *args);

/**
 * Shell command to set the shell mode (enable/disable echo and prompt).
 *
 * @param[in]	*args	the arguments form the commandline
 * @return			result of the command, one of SHELL_PROCESS_xx
 */
int shell_cmd_shell_mode(shell_cmd_args *args);

/**
 * Shell command to enable/disable robots autonomous mode.
 *
 * @param[in]	*args	the arguments form the commandline
 * @return			result of the command, one of SHELL_PROCESS_xx
 */
int shell_cmd_auto_mode(shell_cmd_args *args);

/**
 * Shell command to enable/disable printing of events.
 *
 * @param[in]	*args	the arguments form the commandline
 * @return			result of the command, one of SHELL_PROCESS_xx
 */
int shell_cmd_events(shell_cmd_args *args);

/**
 * Shell command to print out sensor status.
 *
 * @param[in]	*args	the arguments form the commandline
 * @return			result of the command, one of SHELL_PROCESS_xx
 */
int shell_cmd_sensor_status(shell_cmd_args *args);

/**
 * Shell command to print out actor status.
 *
 * @param[in]	*args	the arguments form the commandline
 * @return			result of the command, one of SHELL_PROCESS_xx
 */
int shell_cmd_actor_status(shell_cmd_args *args);

/**
 * Shell command to make robot follow path segment.
 *
 * @param[in]	*args	the arguments form the commandline
 * @return			result of the command, one of SHELL_PROCESS_xx
 */
int shell_cmd_follow_path_segment(shell_cmd_args *args);

#endif

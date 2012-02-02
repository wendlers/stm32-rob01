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

#ifndef __TESTSHELL_H_
#define __TESTSHELL_H_

#include "shell.h"
#include "drive_control.h"

int shell_cmd_help(shell_cmd_args *args);

int shell_cmd_argt(shell_cmd_args *args);

int shell_cmd_eon(shell_cmd_args *args);

int shell_cmd_eoff(shell_cmd_args *args);

int shell_cmd_blink(shell_cmd_args *args);

int shell_cmd_mot(shell_cmd_args *args);

int shell_cmd_bump(shell_cmd_args *args);

int shell_cmd_sensh(shell_cmd_args *args);

int shell_cmd_senen(shell_cmd_args *args);

int shell_cmd_senre(shell_cmd_args *args);

int shell_cmd_but(shell_cmd_args *args);

int shell_cmd_veh(shell_cmd_args *args);

int shell_cmd_actsh(shell_cmd_args *args);

int shell_cmd_fp(shell_cmd_args *args);

int shell_cmd_auto(shell_cmd_args *args);

extern path_segment ps;

extern int follow_path;

extern int mode_auto;

#endif

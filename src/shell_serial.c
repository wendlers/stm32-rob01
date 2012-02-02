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

#include "board.h"
#include "device_serial.h"
#include "shell.h"

void shell_println(char *line)
{
     shell_print(line);
     shell_print("\n\r");
}

void shell_print(char *line)
{
     int   i = 0;

     while(line[i] != 0) {
          serial_send_blocking(&serial, line[i++]);
     }
}

void shell_print_char(char c)
{
     serial_send_blocking(&serial, c);
}

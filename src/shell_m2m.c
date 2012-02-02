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

#include <stdio.h>
#include <libopencm3/stm32/usart.h>

#include "config.h"
#include "board.h"
#include "sensors.h"
#include "actors.h"
#include "delay.h"
#include "device_serial.h"
#include "drive_control.h"
#include "shell_m2m.h"

/**
 * Commands for the M2M shell (machine-to-machine).
 * the description field must have the following format:
 *
 * <description>[;<int|str>:arg 1;<int|str>:arg n; ...]
 *
 */
shell_cmds m2m_shell_cmds = {
     .count 	= 8,
     .cmds	= {
          {
               .cmd		= "h",
               .desc	= "available commands;command:str,parameters:str",
               .func 	= shell_cmd_help,
          },
          {
               .cmd		= "v",
               .desc	= "firmware version;name:str,version:str",
               .func 	= shell_cmd_version,
          },
          {
               .cmd		= "sm",
               .desc	= "shell mode;;echo:int<0|1>,prompt:int<0|1>",
               .func 	= shell_cmd_shell_mode,
          },
          {
               .cmd		= "am",
               .desc	= "disable/enable auto mode;;enable:int<0|1>",
               .func 	= shell_cmd_auto_mode,
          },
          {
               .cmd		= "ev",
               .desc	= "disable/enable events;;enable:int<0|1>",
               .func 	= shell_cmd_events,
          },
          {
               .cmd		= "s",
               .desc	= "sensor status;sensor:str,status:str",
               .func 	= shell_cmd_sensor_status,
          },
          {
               .cmd		= "a",
               .desc	= "actor status;actor:str,status:str",
               .func 	= shell_cmd_actor_status,
          },
          {
               .cmd		= "fps",
               .desc	= "follow path segment;direction:str,wheelcount:int;direction:str<fw|bw|fl|fr|bl|br|tr|tl|br>,wheelcount:int",
               .func 	= shell_cmd_follow_path_segment,
          },
     },
};

extern path_segment ps_to_follow;

extern int follow_path;


int shell_error_param(int count, int expected_count)
{
     if(count != expected_count) {
          shell_error();
          shell_println("wrong number of arguments");
          shell_eod();
          return 1;
     }

     return 0;
}

int shell_get_bool_param(char *str)
{
     int val = 1;

     if(shell_str_cmp("0", str, 1, shell_str_len(str)) == 0) {
          val = 0;
     }

     return val;
}

int shell_cmd_help(shell_cmd_args *args)
{
     int i;

     shell_ok();

     for(i = 0; i < m2m_shell_cmds.count; i++) {
          shell_print((char *)m2m_shell_cmds.cmds[i].cmd);
          shell_print(",");
          shell_println((char *)m2m_shell_cmds.cmds[i].desc);

          // XBee seams not to be fast enough when it gets send a lot of data at once ...
          delay(10000);
     }

     shell_eod();

     return 0;
}

int shell_cmd_version(shell_cmd_args *args)
{
     shell_ok();
     shell_print(APP_NAME);
     shell_print(",");
     shell_println(APP_VERSION);
     shell_eod();

     return 0;
}

int shell_cmd_shell_mode(shell_cmd_args *args)
{
     if(shell_error_param(args->count, 2)) return 1;

     config.shell_do_echo 	= shell_get_bool_param(args->args[0].val);
     config.shell_do_prompt 	= shell_get_bool_param(args->args[1].val);

     shell_ok();
     shell_eod();

     return 0;
}

int shell_cmd_auto_mode(shell_cmd_args *args)
{
     if(shell_error_param(args->count, 1)) return 1;

     config.drive_control_auto = shell_get_bool_param(args->args[0].val);


     if(!config.drive_control_auto) {
          actor_vehicle_break();
     }

     shell_ok();
     shell_eod();

     return 0;
}

int shell_cmd_events(shell_cmd_args *args)
{
     if(shell_error_param(args->count, 1)) return 1;

     config.enable_events = shell_get_bool_param(args->args[0].val);

     shell_ok();
     shell_eod();

     return 0;
}

int shell_cmd_sensor_status(shell_cmd_args *args)
{
     shell_ok();
     shell_print("bumper_left.hit,");
     shell_println_int(sensors.bumper_left.hit);
     shell_print("bumper_right.hit,");
     shell_println_int(sensors.bumper_right.hit);
     shell_print("input_button.state,");
     shell_println_int(sensors.inputbutton.state);

     delay(10000);

     shell_print("odo_left.count,");
     shell_println_int(sensors.odometer_left.count);
     shell_print("odo_left.ppm,");
     shell_println_int(sensors.odometer_left.ppm);
     shell_print("odo_left.lp,");
     shell_println_int(sensors.odometer_left.lp);

     delay(10000);

     shell_print("odo_right.count,");
     shell_println_int(sensors.odometer_right.count);
     shell_print("odo_right.ppm,");
     shell_println_int(sensors.odometer_right.ppm);
     shell_print("odo_right.lp,");
     shell_println_int(sensors.odometer_right.lp);
     shell_eod();

     return 0;
}

int shell_cmd_actor_status(shell_cmd_args *args)
{
     shell_ok();
     shell_print("vehicle_direction,");
     shell_println_int(actors.vehicle.direction);
     shell_eod();

     return 0;
}

int shell_cmd_follow_path_segment(shell_cmd_args *args)
{
     int len;

     ps_to_follow.direction 	= VEHICLE_DIR_NONE;
     ps_to_follow.count	 	= 0;

     if(shell_error_param(args->count, 2)) return 1;

     len = shell_str_len(args->args[0].val);

     if(shell_str_cmp("fw", args->args[0].val, 2, len) == 0) {
          ps_to_follow.direction = VEHICLE_DIR_FORWARD;
     } else if(shell_str_cmp("bw", args->args[0].val, 2, len) == 0) {
          ps_to_follow.direction = VEHICLE_DIR_BACKWARD;
     } else if(shell_str_cmp("fl", args->args[0].val, 2, len) == 0) {
          ps_to_follow.direction = VEHICLE_DIR_FORWARD_LEFT;
     } else if(shell_str_cmp("fr", args->args[0].val, 2, len) == 0) {
          ps_to_follow.direction = VEHICLE_DIR_FORWARD_RIGHT;
     } else if(shell_str_cmp("bl", args->args[0].val, 2, len) == 0) {
          ps_to_follow.direction = VEHICLE_DIR_BACKWARD_LEFT;
     } else if(shell_str_cmp("br", args->args[0].val, 2, len) == 0) {
          ps_to_follow.direction = VEHICLE_DIR_BACKWARD_RIGHT;
     } else if(shell_str_cmp("tl", args->args[0].val, 2, len) == 0) {
          ps_to_follow.direction = VEHICLE_DIR_TURN_LEFT;
     } else if(shell_str_cmp("tr", args->args[0].val, 2, len) == 0) {
          ps_to_follow.direction = VEHICLE_DIR_TURN_RIGHT;
     } else if(shell_str_cmp("st", args->args[0].val, 2, len) == 0) {
          ps_to_follow.direction = VEHICLE_DIR_NONE;
     } else {
          shell_error();
          shell_eod();
          return 1;
     }

     ps_to_follow.count = shell_parse_int(args->args[1].val);

     while(follow_path) delay(100);

     follow_path = 1;

     shell_ok();
     shell_eod();

     return 0;
}

void shell_prompt()
{
     if(config.shell_do_prompt) {
          shell_print(SHELL_PROMPT);
     };
}

int shell_process(char *cmd_line)
{
     return shell_process_cmds(&m2m_shell_cmds, cmd_line);
}

void usart1_isr(void)
{
     static char cmd_line[SHELL_MAX_CMD_LINE];
     static int  i = 0;

     /* Check if we were called because of RXNE. */
     if (((USART_CR1(USART1) & USART_CR1_RXNEIE) != 0) &&
               ((USART_SR(USART1) & USART_SR_RXNE) != 0)) {

          // empty rx reg.
          int c = serial_recv(&serial);

          if(config.shell_do_echo) {
               serial_send_blocking(&serial, c);
          }

          if(c == '\r') {
               shell_println("");
               if(i > 0) {
                    cmd_line[i] = 0;

                    if(shell_process(cmd_line) != SHELL_PROCESS_OK) {
                         shell_error();
                         shell_eod();
                    }
               }
               i = 0;

               shell_prompt();

          } else if( c != '\n') {
               cmd_line[i++] = (char )(c);
          }

          if(i > SHELL_MAX_CMD_LINE - 1) i = 0;
     }
}

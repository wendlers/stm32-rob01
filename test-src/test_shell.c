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

#include "board.h"
#include "delay.h"
#include "sensors.h"
#include "actors.h"
#include "drive_control.h"
#include "device_serial.h"
#include "device_led.h"
#include "device_dcmotor.h"
#include "device_button.h"
#include "device_bumpersensor.h"
#include "test_shell.h"

shell_cmds test_shell_cmds = {
     .count 	= 15,
     .cmds	= {
          {
               .cmd		= "help",
               .desc	= " Print available shell commands",
               .func 	= shell_cmd_help,
          },
          {
               .cmd		= "eon",
               .desc	= "  Turn echo on",
               .func 	= shell_cmd_eon,
          },
          {
               .cmd		= "eoff",
               .desc	= " Turn echo off",
               .func 	= shell_cmd_eoff,
          },
          {
               .cmd		= "argt",
               .desc	= " Print out given arguments",
               .func 	= shell_cmd_argt,
          },
          {
               .cmd		= "blink",
               .desc	= "Blink the on-board LEDs",
               .func 	= shell_cmd_blink,
          },
          {
               .cmd		= "mot",
               .desc	= "  Control the DC motor. Takes 2 arguments: <motor l|r> <mode cw|ccw|f|b>",
               .func 	= shell_cmd_mot,
          },
          {
               .cmd		= "bump",
               .desc	= " Check bumper sensors. Takes 1 argument: <sensor l|r>",
               .func 	= shell_cmd_bump,
          },
          {
               .cmd		= "sensh",
               .desc	= "Show current sensor states",
               .func 	= shell_cmd_sensh,
          },
          {
               .cmd		= "senen",
               .desc	= "Enable/disable sensor measurement. Takes 1 argument: <enable 0|1>",
               .func 	= shell_cmd_senen,
          },
          {
               .cmd		= "senre",
               .desc	= "Reset sensor values to zero",
               .func 	= shell_cmd_senre,
          },
          {
               .cmd		= "but",
               .desc	= "  Check input button",
               .func 	= shell_cmd_but,
          },
          {
               .cmd		= "veh",
               .desc	= "  Drive vehicle. Takes 1 argument: <direction fw|bw|fl|fr|bl|br|tl|tr|st>",
               .func 	= shell_cmd_veh,
          },
          {
               .cmd		= "actsh",
               .desc	= "Show current actor states.",
               .func 	= shell_cmd_actsh,
          },
          {
               .cmd		= "fp",
               .desc	= "   Follow path segment. Takes 2 arguments: <direction fw|biw|fl|fr|bl|br|tl|tr|st> <count n>",
               .func 	= shell_cmd_fp,
          },
          {
               .cmd		= "auto",
               .desc	= "   Enable/disable auto mode. Takes 1 arguments: <enable 0|1>",
               .func 	= shell_cmd_auto,
          },
     },
};

int do_echo = 1;

path_segment ps = {
     .direction = VEHICLE_DIR_NONE,
     .count     = 0,
};

int follow_path = 0;

int mode_auto = 0;

int shell_cmd_help(shell_cmd_args *args)
{
     int i;

     shell_ok();

     shell_println("RoborControlFW - Test Shell Commads");

     for(i = 0; i < test_shell_cmds.count; i++) {
          shell_print("- ");
          shell_print((char *)test_shell_cmds.cmds[i].cmd);
          shell_print("  ");
          shell_println((char *)test_shell_cmds.cmds[i].desc);

          // XBee seams not to be fast enough when it gets send a lot of data at once ...
          delay(1000);
     }

     shell_eod();

     return 0;
}

int shell_cmd_eon(shell_cmd_args *args)
{
     shell_ok();

     shell_println("echo on");

     do_echo = 1;

     shell_eod();

     return 0;
}

int shell_cmd_eoff(shell_cmd_args *args)
{
     shell_ok();

     shell_println("echo off");

     do_echo = 0;

     shell_eod();

     return 0;
}

int shell_cmd_argt(shell_cmd_args *args)
{
     int i;

     shell_ok();

     shell_print_int(args->count);
     shell_println(" args given:");

     for(i = 0; i < args->count; i++) {
          shell_print_int(i);
          shell_print("  ");
          shell_println(args->args[i].val);
     }

     shell_eod();

     return 0;
}

int shell_cmd_blink(shell_cmd_args *args)
{
     int i;

     shell_ok();

     shell_println("Blink LEDs");

     for(i = 0; i < 10; i++) {
          led_toggle(&status_led1);
          led_toggle(&status_led2);
          delay(800000);
     }

     shell_eod();

     return 0;
}

int shell_cmd_mot(shell_cmd_args *args)
{
     int len;

     device *dcmotor;

     if(args->count != 2) {
          shell_error();
          shell_println("cmd mot takes 2 arguments: <motor l|r> <mode cw|ccw|f|b>");
          shell_eod();
          return 1;
     }

     len = shell_str_len(args->args[0].val);

     if(shell_str_cmp("l", args->args[0].val, 1, len) == 0) {
          dcmotor = &dcmotor_left;
     } else if(shell_str_cmp("r", args->args[0].val, 1, len) == 0) {
          dcmotor = &dcmotor_right;
     } else {
          shell_error();
          shell_println("cmd mot takes 2 arguments: <motor l|r> <mode cw|ccw|f|b>");
          shell_eod();
          return 1;
     }

     len = shell_str_len(args->args[1].val);

     if(shell_str_cmp("cw", args->args[1].val, 2, len) == 0) {
          dcmotor_cw(dcmotor);
     } else if(shell_str_cmp("ccw", args->args[1].val, 3, len) == 0) {
          dcmotor_ccw(dcmotor);
     } else if(shell_str_cmp("f", args->args[1].val, 1, len) == 0) {
          dcmotor_float(dcmotor);
     } else if(shell_str_cmp("b", args->args[1].val, 1, len) == 0) {
          dcmotor_break(dcmotor);
     } else {
          shell_error();
          shell_println("cmd mot takes 2 arguments: <motor l|r> <mode cw|ccw|f|b>");
          shell_eod();
          return 1;
     }

     shell_ok();

     shell_print("Setting mode of DC motor ");
     shell_print(args->args[0].val);
     shell_print(" to ");
     shell_println(args->args[1].val);

     shell_eod();

     return 0;
}

int shell_cmd_bump(shell_cmd_args *args)
{
     int max_wait = 5000000;
     int len;

     device *bumpersensor;

     if(args->count != 1) {
          shell_error();
          shell_println("cmd bump takes 1 argument: <sensor l|r>");
          shell_eod();
          return 1;
     }

     len = shell_str_len(args->args[0].val);

     if(shell_str_cmp("l", args->args[0].val, 1, len) == 0) {
          bumpersensor = &bumpersensor_left;
     } else if(shell_str_cmp("r", args->args[0].val, 1, len) == 0) {
          bumpersensor = &bumpersensor_right;
     } else {
          shell_error();
          shell_println("cmd bump takes 1 argument: <sensor l|r>");
          shell_eod();
          return 1;
     }

     shell_ok();

     shell_print("Waiting for obstacle on bumpersensor ");
     shell_println(args->args[0].val);

     while(!bumpersensor_obstacle_detected(bumpersensor) && max_wait-- > 0);

     if(max_wait > 0) {
          shell_println("obstacle detected successfully");
     } else {
          shell_println("timted out while waiting for bumpersensor");
     }

     shell_eod();

     return 0;
}

int shell_cmd_sensh(shell_cmd_args *args)
{
     shell_ok();

     shell_println("Current sensor sates:");

     shell_println("- Bumper Sensors");
     shell_print("  * hit left : ");
     shell_println_int(sensors.bumper_left.hit);
     shell_print("  * hit right: ");
     shell_println_int(sensors.bumper_right.hit);

     delay(1000);

     shell_println("- Odometrie Sensors");
     shell_print("  * count left : ");
     shell_println_int(sensors.odometer_left.count);
     shell_print("  * ppm left : ");
     shell_println_int(sensors.odometer_left.ppm);
     shell_print("  * lp left : ");
     shell_println_int(sensors.odometer_left.lp);

     delay(1000);

     shell_print("  * count right: ");
     shell_println_int(sensors.odometer_right.count);
     shell_print("  * ppm right : ");
     shell_println_int(sensors.odometer_right.ppm);
     shell_print("  * lp right : ");
     shell_println_int(sensors.odometer_right.lp);

     shell_println("- Input Button");
     shell_print("  * state : ");
     shell_println_int(sensors.inputbutton.state);

     shell_eod();

     return 0;
}

int shell_cmd_senen(shell_cmd_args *args)
{
     int len;
     int en;

     if(args->count != 1) {
          shell_error();
          shell_println("cmd senen takes 1 argument: <enable 0|1>");
          shell_eod();
          return 1;
     }

     len = shell_str_len(args->args[0].val);

     if(shell_str_cmp("1", args->args[0].val, 1, len) == 0) {
          en = 1;
     } else if(shell_str_cmp("0", args->args[0].val, 1, len) == 0) {
          en = 0;
     } else {
          shell_error();
          shell_println("cmd senen takes 1 argument: <enable 0|1>");
          shell_eod();
          return 1;
     }

     sensors_en.bumper 		= en;
     sensors_en.odometer 	= en;
     sensors_en.inputbutton 	= en;

     shell_ok();

     if(en == 1) {
          shell_println("sensors enabled");
     } else {
          shell_println("sensors disabled");
     }

     shell_eod();

     return 0;
}

int shell_cmd_senre(shell_cmd_args *args)
{
     sensor_reset_bumper();
     sensor_reset_odometer();
     sensor_reset_inputbutton();

     shell_ok();

     shell_println("sensor values reset");

     shell_eod();

     return 0;
}

int shell_cmd_but(shell_cmd_args *args)
{
     int max_wait = 5000000;

     shell_ok();

     shell_println("Waiting for buttonpressed on inputbutton");

     while(!button_is_pressed(&button_user1) && max_wait-- > 0);

     if(max_wait > 0) {
          shell_println("button pressed detected successfully");
     } else {
          shell_println("timed out while waiting for button pressed");
     }

     shell_eod();

     return 0;
}

int shell_cmd_veh(shell_cmd_args *args)
{
     int len;

     if(args->count != 1) {
          shell_error();
          shell_println("cmd veh takes 1 argument: <direction fw|bw|fl|fr|bl|br|tl|tr|st>");
          shell_eod();
          return 1;
     }

     len = shell_str_len(args->args[0].val);

     if(shell_str_cmp("fw", args->args[0].val, 2, len) == 0) {
          actor_vehicle_forward();
     } else if(shell_str_cmp("bw", args->args[0].val, 2, len) == 0) {
          actor_vehicle_backward();
     } else if(shell_str_cmp("fl", args->args[0].val, 2, len) == 0) {
          actor_vehicle_forward_left();
     } else if(shell_str_cmp("fr", args->args[0].val, 2, len) == 0) {
          actor_vehicle_forward_right();
     } else if(shell_str_cmp("bl", args->args[0].val, 2, len) == 0) {
          actor_vehicle_backward_left();
     } else if(shell_str_cmp("br", args->args[0].val, 2, len) == 0) {
          actor_vehicle_backward_right();
     } else if(shell_str_cmp("tl", args->args[0].val, 2, len) == 0) {
          actor_vehicle_turn_left();
     } else if(shell_str_cmp("tr", args->args[0].val, 2, len) == 0) {
          actor_vehicle_turn_right();
     } else if(shell_str_cmp("st", args->args[0].val, 2, len) == 0) {
          actor_vehicle_break();
     } else {
          shell_error();
          shell_println("cmd veh takes 1 argument: <direction fw|bw|fl|fr|bl|br|tl|tr|st>");
          shell_eod();
          return 1;
     }

     shell_ok();

     shell_print("moving vehicle: ");
     shell_print(args->args[0].val);
     shell_print(" (");
     shell_print_int(actors.vehicle.direction);
     shell_println(")");

     shell_eod();

     return 0;
}

int shell_cmd_actsh(shell_cmd_args *args)
{
     shell_ok();

     shell_println("Current actor sates:");

     shell_println("- Vehicle");
     shell_print("  * direction: ");
     shell_println_int(actors.vehicle.direction);

     shell_eod();

     return 0;
}

extern void drive_control_follow_path(path_segment *ps);

int shell_cmd_fp(shell_cmd_args *args)
{
     int len;

     ps.direction = VEHICLE_DIR_NONE;
     ps.count	 = 0;

     if(args->count != 2) {
          shell_error();
          shell_println("cmd fp takes 2 arguments: <direction fw|bw|fl|fr|bl|br|tl|tr|st> <count n>");
          shell_eod();
          return 1;
     }

     len = shell_str_len(args->args[0].val);

     if(shell_str_cmp("fw", args->args[0].val, 2, len) == 0) {
          ps.direction = VEHICLE_DIR_FORWARD;
     } else if(shell_str_cmp("bw", args->args[0].val, 2, len) == 0) {
          ps.direction = VEHICLE_DIR_BACKWARD;
     } else if(shell_str_cmp("fl", args->args[0].val, 2, len) == 0) {
          ps.direction = VEHICLE_DIR_FORWARD_LEFT;
     } else if(shell_str_cmp("fr", args->args[0].val, 2, len) == 0) {
          ps.direction = VEHICLE_DIR_FORWARD_RIGHT;
     } else if(shell_str_cmp("bl", args->args[0].val, 2, len) == 0) {
          ps.direction = VEHICLE_DIR_BACKWARD_LEFT;
     } else if(shell_str_cmp("br", args->args[0].val, 2, len) == 0) {
          ps.direction = VEHICLE_DIR_BACKWARD_RIGHT;
     } else if(shell_str_cmp("tl", args->args[0].val, 2, len) == 0) {
          ps.direction = VEHICLE_DIR_TURN_LEFT;
     } else if(shell_str_cmp("tr", args->args[0].val, 2, len) == 0) {
          ps.direction = VEHICLE_DIR_TURN_RIGHT;
     } else if(shell_str_cmp("st", args->args[0].val, 2, len) != 0) {
          shell_error();
          shell_println("cmd fp takes 2 arguments: <direction fw|bw|fl|fr|bl|br|tl|tr|st> <count n>");
          shell_eod();
          return 1;
     }

     ps.count = shell_parse_int(args->args[1].val);

     while(follow_path);

     follow_path = 1;

     shell_ok();

     shell_print("following path segment: ");
     shell_print(args->args[0].val);
     shell_print(" ");
     shell_println_int(ps.count);

     shell_eod();

     return 0;
}

int shell_cmd_auto(shell_cmd_args *args)
{
     int len;
     int en;

     if(args->count != 1) {
          shell_error();
          shell_println("cmd auto takes 1 argument: <enable 0|1>");
          shell_eod();
          return 1;
     }

     len = shell_str_len(args->args[0].val);

     if(shell_str_cmp("1", args->args[0].val, 1, len) == 0) {
          en = 1;
     } else if(shell_str_cmp("0", args->args[0].val, 1, len) == 0) {
          en = 0;
     } else {
          shell_error();
          shell_println("cmd auto takes 1 argument: <enable 0|1>");
          shell_eod();
          return 1;
     }

     shell_ok();

     mode_auto = en;

     if(en == 1) {
          shell_println("automode enabled");
     } else {
          shell_println("automode disabled");
     }

     shell_eod();

     return 0;
}

void shell_prompt()
{
     shell_print("$> ");
}

int shell_process(char *cmd_line)
{
     return shell_process_cmds(&test_shell_cmds, cmd_line);
}

void usart1_isr(void)
{
     static char cmd_line[SHELL_MAX_CMD_LINE];
     static int  i = 0;

     int c;

     /* Check if we were called because of RXNE. */
     if (((USART_CR1(USART1) & USART_CR1_RXNEIE) != 0) &&
               ((USART_SR(USART1) & USART_SR_RXNE) != 0)) {

          // empty rx reg.
          c = serial_recv(&serial);

          if(do_echo) {
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

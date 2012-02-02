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

/*
 *
 */

#include <libopencm3/stm32/f1/rcc.h>
#include <libopencm3/stm32/f1/rtc.h>

#include "board.h"
#include "delay.h"
#include "sensors.h"
#include "actors.h"
#include "test_shell.h"
#include "drive_control.h"

int main(void)
{
     board_init();
     board_init_rtc();

     shell_println("");
     shell_println("RobotControl FW Test Shell - use help for available commands");
     shell_println("");

     // 24MHz / 8 => 3000000 counts per second
     // 3000000/3000 = 1000 overflows per second - every 1ms one interrupt
     board_init_systick(STK_CTRL_CLKSOURCE_AHB_DIV8, 3000);

     follow_path = 0;

     while(1) {
          // delay(10000);
          delay(1000);

          if(sensors.inputbutton.state) {
               if(mode_auto == 0) {
                    mode_auto = 1;
               } else {
                    mode_auto = 0;
                    actor_vehicle_break();
               }
               while(sensors.inputbutton.state) delay(100);
          }

          if(follow_path) {

               // shell_println_int(rtc_get_counter_val());

               drive_control_follow_path(&ps);
               follow_path = 0;
          } else if(mode_auto) {
               drive_control_auto();
          }
     }

     return 0;
}

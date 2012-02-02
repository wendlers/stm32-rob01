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

#ifndef __CONFIG_H_
#define __CONFIG_H_

/**
 * Name of the application
 */
#define APP_NAME		"RobotControlFW"


/**
 * Version of the application
 */
#define APP_VERSION		"0.1"

/**
 * Shell prompt to dispaly (if enabled)
 */
#define SHELL_PROMPT	"rc> "

/**
 * Application configuration setable through shell etc.
 */
typedef struct {
     /**
      * Enable/disable echo of characters received on shell
      */
     int shell_do_echo;

     /**
      * Enable/disable displaying of shell prompt
      */
     int shell_do_prompt;

     /**
      * Enable/disable auto drive control
      */
     int drive_control_auto;

     /**
      * Enable/disable events dispalyed on the shell
      */
     int enable_events;
} app_config;

/**
 * The global {@link app_config} instance
 */
extern app_config config;

#endif

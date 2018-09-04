/*
 * Logger.cpp: Uses introspec.h logging system to maintain a record of robot state for debugging purposes.
 * After log is produced, it is sent over FTP to a computer, where the "read" command-line tool decodes the
 * "Log.bin" binary into ASCII and organizes it into a .csv file. The .csv file can be read into Matlab as
 * column vectors, and a Matlab script forms "scenes," or robot states, and forms an animation of the robot's
 * navigation by stringing these scenes together.
 *  Created on: Jun 11, 2018
 *      Author: David Linn
 */

#include "Logger.h"
#include "introspec.h"
#include <ucosmcfc.h>
#include <ucos.h>
#include <constants.h>
#include <stdio.h>

#define LOG_PRIO (MAIN_PRIO-1)

//Object logged once per log cycle
START_INTRO_OBJ(MainLogObject,"Log")
int_element time{"time"};
float_element a{"a"};
uint16_element b{"b"};
END_INTRO_OBJ;

MainLogObject mainLog;
extern int secs;
extern float a;
extern uint16_t b;

void Logger::logLoop(void*) {
	while (1) {
		if (secs < 20) {
			printf("Logging\n");
			mainLog.time=secs;
			mainLog.a=a;
			mainLog.b=b;
			mainLog.Log();
		}
		OSTimeDly(TICKS_PER_SECOND/2); //Log at 2 Hz
	}
}

void Logger::logBegin() {
    LogFileVersions();
	OSSimpleTaskCreatewName(logLoop,LOG_PRIO,"Log");
}

void Logger::logWrite(char* buf) {
	LogMessage(buf);
}

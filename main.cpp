#include <predef.h>
#include <ctype.h>
#include <startnet.h>
#include <autoupdate.h>
#include <dhcpclient.h>
#include <smarttrap.h>
#include <taskmon.h>
#include <NetworkDebug.h>
#include <constants.h>
#include "Logger.h"
#include "introspec.h"

#define FTP_PRIO (MAIN_PRIO-2)
LOGFILEINFO //Logs the version of this file
const char *AppName = "Real-time Logging Example";

int secs = 0;
float a = 1;
uint16_t b = 1;

extern "C" void UserMain(void *pd)
{
    //Basic network initialization
	InitializeStack();
    if (EthernetIP.IsNull()) GetDHCPAddress();
    iprintf("IP Address: ");
    ShowIP(EthernetIP);
    iprintf("\r\n");
    OSChangePrio(MAIN_PRIO);
    EnableAutoUpdate();
    #ifndef _DEBUG
    EnableSmartTraps();
    #endif
    #ifdef _DEBUG
    InitializeNetworkGDB_and_Wait();
    #endif

	//FTP Server for Log Transmission
	InitLogFtp(FTP_PRIO);

	//Start Logging
	bLog = true;
    Logger::logBegin();

    while (1)
    {
        printf("Updating Main\n");
    	++secs;
        a = a/2;
        b *= 2;
    	OSTimeDly(TICKS_PER_SECOND); //Update main every second
    }
}

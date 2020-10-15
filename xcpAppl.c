/*----------------------------------------------------------------------------
| File:
|   xcpAppl.cpp
|
| Description:
|   XCP protocol layer application callbacks
|   DAQ clock, all other callbacks are implemented as macros
|   Demo for XCP on Ethernet (UDP)
|   Linux (Raspberry Pi) Version
 ----------------------------------------------------------------------------*/

#include "xcpAppl.h"


// Wall clock updated at every AppXcpTimer
volatile vuint32 gTimer = 0;


/**************************************************************************/
// ApplXcpTimer()
// ApplXcpTimerInit()
// Platform and implementation specific functions for the XCP driver
// DAQ clock
/**************************************************************************/

/* Compile with:   -lrt */

static struct timespec gts0;
static struct timespec gtr;

void ApplXcpTimerInit( void )
{    
    assert(sizeof(long long) == 8);
    clock_getres(CLOCK_REALTIME, &gtr);
    assert(gtr.tv_sec == 0);
    assert(gtr.tv_nsec == 1);
    clock_gettime(CLOCK_REALTIME, &gts0);

#if defined ( XCP_ENABLE_TESTMODE )
    if (gXcpDebugLevel >= 1) {
        printf("clock resolution %lds,%ldns\n", gtr.tv_sec, gtr.tv_nsec);
        printf("clock %lds,%ldns\n", gts0.tv_sec, gts0.tv_nsec);
    }
#endif

}

// Free runing clock with 10ns tick
// 1ns with overflow every 4s is critical for CANape measurement start time offset calculation
vuint32 ApplXcpTimer(void) {

    struct timespec ts; 
    
    clock_gettime(CLOCK_REALTIME, &ts);
    gTimer = (vuint32)( ( (unsigned long long)(ts.tv_sec-gts0.tv_sec) * 1000000L ) + (ts.tv_nsec/1000) ); // us
    return gTimer;  
}




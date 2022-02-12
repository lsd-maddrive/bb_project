#include "tests.h"
#include "common.h"
#include "logger.h"
#include "robot_odometry.h"

typedef enum {
    IDLE = 0,
    RUN = 1
}system_state;

system_state    cur_system_state = IDLE;

static SerialDriver  *mainSerial      = &SD3;

static THD_WORKING_AREA(waStateGetter, 256);
static THD_FUNCTION(StateGetter, arg)
{
    arg = arg;

    uint8_t     cmd  = 0;
    systime_t   time = chVTGetSystemTimeX( );
    while( 1 )
    {
        cmd = sdGetTimeout( mainSerial, TIME_IMMEDIATE );

        if( cmd == 185 )
        {
            cur_system_state = RUN;
        }
        else if( cmd == 186 )
        {
            cur_system_state = IDLE;
        }

        time = chThdSleepUntilWindowed( time, time + MS2ST( 50 ) );
    }
}

/**
 * @brief   Initialization of base units for main programm
 */
void mainUnitsInit( void )
{
    debug_stream_init();
    robotOdometryInit();


    chThdCreateStatic(waStateGetter, sizeof(waStateGetter), NORMALPRIO - 1, StateGetter, NULL);
}

/**
 * @brief   Main control program
 */
void mainControlTask( void )
{
    systime_t time = chVTGetSystemTime();
    while( 1 )
    {
        if( cur_system_state == IDLE )
        {
            robotOdometryResetPermeation( );
        }
        else if( cur_system_state == RUN )
        {
            robotOdometrySetPermeation( );
        }

        time = chThdSleepUntilWindowed( time, time + MS2ST( 25 ) );
    }
}

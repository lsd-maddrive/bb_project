#include <tests.h>
#include "odometry.h"

/*
 * @brief   Test motor speed 
 */
void testMotorSpeed( void )
{
    debug_stream_init( ); 
    odometryInit( ); 

    float   test_motor_speed    = 0; 

    systime_t   time = chVTGetSystemTimeX( );
    while( true )
    {
        test_motor_speed    = odometryGetEncoderSpeed( REVS_PER_SEC ); 

        dbgprintf( "SP: (%d)\n\r", (int)test_motor_speed ); 

        time = chThdSleepUntilWindowed( time, time + MS2ST( 100 ) );
    }
}
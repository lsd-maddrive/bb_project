#include <tests.h>
#include "odometry.h"

/*
 * @brief   Test motor speed 
 */
void testMotorSpeed( void )
{
    debug_stream_init( ); 
    odometryInit( ); 

    float   test_motor1_speed    = 0;
    float   test_motor2_speed    = 0;
    float   test_motor3_speed    = 0;

    systime_t   time = chVTGetSystemTimeX( );
    while( true )
    {
        test_motor1_speed    = odometryGetEncoderSpeed( 1, REVS_PER_SEC );
        test_motor2_speed    = odometryGetEncoderSpeed( 2, REVS_PER_SEC );
        test_motor3_speed    = odometryGetEncoderSpeed( 3, REVS_PER_SEC );

        dbgprintf( "SP1: %d\tSP2: %d\tSP3: %d\n\r",
                   (int)test_motor1_speed, (int)test_motor2_speed,
                   (int)test_motor3_speed );

        time = chThdSleepUntilWindowed( time, time + MS2ST( 300 ) );
    }
}

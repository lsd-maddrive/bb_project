#include <tests.h>
#include "robot_odometry.h"

/*
 * @brief   Test robot (kinematic) odometry unit
 */
void testRobotOdometry ( void )
{
    robotOdometryInit();
    debug_stream_init();

    float   test_v_x    = 0;
    float   test_v_y    = 0;
    float   test_w      = 0;


    systime_t   time = chVTGetSystemTimeX( );
    while( true )
    {
        char rcv_data   = sdGetTimeout( &SD3, TIME_IMMEDIATE );
        switch( rcv_data )
        {
            case 'x':  // change only test_v_x
              test_v_x = 0.2;
              test_v_y = 0;
              test_w = 0;
              break;

            case 'y':  // change only test_v_y
              test_v_x = 0;
              test_v_y = 0.2;
              test_w = 0;
              break;

            case 'w':  // change only test_w
              test_v_x = 0;
              test_v_y = 0;
              test_w = 0.6;
              break;

            case ' ':  // full stop
              test_v_x    = 0;
              test_v_y    = 0;
              test_w      = 0;
              break;

            default:
              break;
        }

        robotOdometrySetSpeed( test_v_x, test_v_y, test_w );

        dbgprintf( "Vx: %d\tVy: %d\tW: %d\n\r",
                   (int)(test_v_x * 1000),
                   (int)(test_v_y * 1000),
                   (int)(test_w * 1000)
                   );

        time = chThdSleepUntilWindowed( time, time + MS2ST( 300 ) );
    }
}


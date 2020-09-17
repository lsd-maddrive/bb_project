#include <tests.h>
#include "robot_odometry.h"

// TODO: remake test!!! with GYRO
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
    float   test_speed  = 0.2f;
    float   test_angle  = 10.0f;


    systime_t   time = chVTGetSystemTimeX( );
    while( true )
    {
        char rcv_data   = sdGetTimeout( &SD3, TIME_IMMEDIATE );
        switch( rcv_data )
        {
            case 'x':  // change only test_v_x (plus)
              test_v_x = test_speed;
              test_v_y = 0;
              test_w = 0;
              break;


            case 'm':  // change only test_v_x (minus)
              test_v_x = -test_speed;
              test_v_y = 0;
              test_w = 0;
              break;


            case 'y':  // change only test_v_y
              test_v_x = 0;
              test_v_y = test_speed;
              test_w = 0;
              break;

            case 'n':  // change only test_v_y (minus)
              test_v_x = 0;
              test_v_y = -test_speed;
              test_w = 0;
              break;


            case 'w':  // change only test_w (plus)
              test_v_x = 0;
              test_v_y = 0;
              // BE CAREFUL! INTEGRAL OF ANGLE
              test_w += test_angle;
              // cur_angle = setAngleIntegral(test_angle);
              break;

            case 'q':  // change only test_w (minus)
              test_v_x = 0;
              test_v_y = 0;
              test_w -= test_angle;
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
// TODO: FIX IT
        float err = getPropError( );
        float ctr = getSpeedA(  );

        dbgprintf("E: %d\tC: %d\tW: %d\n\r",
                  (int)(err * 10),
                  (int)(ctr * 100),
                  (int)(test_w));

// TODO: FIX IT
//        dbgprintf( "Vx: %d\tVy: %d\tW: %d\n\r",
//                   (int)(test_v_x * 1000),
//                   (int)(test_v_y * 1000),
//                   (int)(cur_angle)
//                   );

        time = chThdSleepUntilWindowed( time, time + MS2ST( 100 ) );
    }
}



#include <tests.h>
#include "robot_odometry.h"
#include "gyroscope.h"

static const SerialConfig sdcfg = {
  .speed = 115200,
  .cr1 = 0, .cr2 = 0, .cr3 = 0
};

/*
 * @brief   Simple joystick test (without control system)
 */
void testJoystick( void )
{
    debug_stream_init();

    sdStart( &SD6, &sdcfg );
    palSetPadMode( GPIOG, 14, PAL_MODE_ALTERNATE(8) );   // TX
    palSetPadMode( GPIOG, 9,  PAL_MODE_ALTERNATE(8) );   // RX

    float buf[3] = {0, 0, 0};

    systime_t   time = chVTGetSystemTimeX( );
    while( true )
    {
        sdReadTimeout( &SD6, (uint8_t*) &buf, 12, TIME_IMMEDIATE );

        dbgprintf("x: %d\ty: %d\tw: %d\n\r",
                  (int)(buf[0] * 100), (int)(buf[1] * 100), (int)(buf[2] * 100));

        time = chThdSleepUntilWindowed( time, time + MS2ST( 100 ) );
    }
}

// TODO: add comments
void testRobotWithJoystick( void )
{
    gyroscopeInit( NORMALPRIO );
    robotOdometryInit();

    sdStart( &SD6, &sdcfg );
    palSetPadMode( GPIOG, 14, PAL_MODE_ALTERNATE(8) );   // TX
    palSetPadMode( GPIOG, 9,  PAL_MODE_ALTERNATE(8) );   // RX

    float buf[3] = {0, 0, 0};

    systime_t   time = chVTGetSystemTimeX( );
    while( true )
    {
      sdReadTimeout( &SD6, (uint8_t*) &buf, 12, TIME_IMMEDIATE );

      robotOdometryAddAngle(buf[2]);
      robotOdometrySetSpeed(buf[0], buf[1]);

      time = chThdSleepUntilWindowed( time, time + MS2ST( 10 ) );
    }
}

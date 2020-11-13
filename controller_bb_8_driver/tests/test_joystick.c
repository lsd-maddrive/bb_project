#include <tests.h>
#include "robot_odometry.h"
#include "lld_gyroscope.h"
#include "logger.h"

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
        sdReadTimeout( &SD3, (uint8_t*) &buf, 12, TIME_IMMEDIATE );

        chprintf( (BaseSequentialStream *)&SD6, "x: %d\ty: %d\tw: %d\n\r",
                  (int32_t)(buf[0] * 100),
                  (int32_t)(buf[1] * 100),
                  (int32_t)(buf[2] * 100)
                  );

        time = chThdSleepUntilWindowed( time, time + MS2ST( 100 ) );
    }
}

/*
 * @brief   Test control robot via joystick
 * @note    Data transferring via USB (SD3)
 * */
void testRobotWithJoystick( void )
{
    robotOdometryInit();
    debug_stream_init();

    sdStart( &SD6, &sdcfg );
    palSetPadMode( GPIOG, 14, PAL_MODE_ALTERNATE(8) );   // TX
    palSetPadMode( GPIOG, 9,  PAL_MODE_ALTERNATE(8) );   // RX

//    // TODO: remove after testing
//    sdStart( &SD6, &sdcfg );
//    palSetPadMode( GPIOG, 14, PAL_MODE_ALTERNATE(8) );   // TX
//    palSetPadMode( GPIOG, 9,  PAL_MODE_ALTERNATE(8) );   // RX

    float log[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    float buf[3] = {0, 0, 0};
    uint16_t    time_delta  = 100;
    float       time_k      = (float)time_delta * 0.001;

    chprintf( (BaseSequentialStream *)&SD6, "Start test!" );
    systime_t time = chVTGetSystemTimeX( );
    while( true )
    {
      sdReadTimeout( &SD3, (uint8_t*) &buf, 12, TIME_IMMEDIATE );

      // 0.1 = 100 ms -> 1 ms
      robotOdometrySetSpeed(buf[0], buf[1], buf[2], time_k );

      //Send all data to python

      log[0] = (float)chVTGetSystemTimeX( );
      log[1] = getSetAngle();
      log[2] = getGyroAngle( GYRO_AXIS_Z );
      log[3] = buf[0];
      log[4] = buf[1];
      log[5] = buf[2];
      log[6] = getVelocityXLocal();
      log[7] = getVelocityYLocal();
      log[8] = odometryGetWheelSpeed(A, REVS_PER_SEC);
      log[9] = odometryGetWheelSpeed(B, REVS_PER_SEC);
      log[10] = odometryGetWheelSpeed(C, REVS_PER_SEC);


      sendLog( &SD3, log, 11);


      time = chThdSleepUntilWindowed( time, time + MS2ST( time_delta ) );
    }
}

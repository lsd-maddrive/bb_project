#include <tests.h>
#include "lld_control.h"
#include "odometry.h"
#include "wheel_control.h"


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
        test_motor1_speed    = odometryGetEncoderSpeed( A, REVS_PER_SEC );
        test_motor2_speed    = odometryGetEncoderSpeed( B, REVS_PER_SEC );
        test_motor3_speed    = odometryGetEncoderSpeed( C, REVS_PER_SEC );

        dbgprintf( "SP1: %d\tSP2: %d\tSP3: %d\n\r",
                   (int)test_motor1_speed, (int)test_motor2_speed,
                   (int)test_motor3_speed );

        time = chThdSleepUntilWindowed( time, time + MS2ST( 300 ) );
    }
}

#define WHEEL_SPEED_MATLAB

static const SerialConfig sdcfg = {
  .speed = 115200,
  .cr1 = 0, .cr2 = 0, .cr3 = 0
};


/*
 * @brief   Test wheel speed (A!)
 * @note    Two options are available:
 *              - debug via Terminal (comment   define WHEEL_SPEED_MATLAB)
 *              - debug via Matlab   (uncomment define WHEEL_SPEED_MATLAB)
 *                in Matlab only data about wheel A is sended
 *                Consumption:
 *                  wheel A data is enough
 *                  because all motors (wheels) are the same
 */
void testWheelSpeed( void )
{

    lldControlInit( );
    odometryInit( );
    debug_stream_init( );

    odometrySpeedValue_t    test_wheel_A        = 0;

#ifdef WHEEL_SPEED_MATLAB

    lldControlValue_t       test_delta_prc      = 10;
    lldControlValue_t       test_duty_A_prc     = 0;
    lldControlValue_t       test_duty_B_prc     = 0;
    lldControlValue_t       test_duty_C_prc     = 0;

    odometrySpeedValue_t    test_wheel_A_raw    = 0;

    uint8_t                 matlab_start_flag   = 0;

    int16_t                 matlab_wheel_A      = 0;
    int16_t                 matlab_wheel_A_raw  = 0;
#endif

    systime_t   time = chVTGetSystemTimeX( );
    while( true )
    {
#ifdef WHEEL_SPEED_MATLAB
        test_wheel_A_raw  = odometryGetWheelSpeedRaw( A, REVS_PER_SEC );
#endif
        test_wheel_A      = odometryGetWheelSpeed( A, REVS_PER_SEC );


#ifdef WHEEL_SPEED_MATLAB
        char rc_data         = sdGetTimeout( &SD3, TIME_IMMEDIATE );

        switch( rc_data )
        {
            case 's':   // start
                matlab_start_flag = 1;
                break;

            case 'f':   // increase speed wheel A
                test_duty_A_prc += test_delta_prc;
                break;

            case 'd':   // decrease speed wheel A
                test_duty_A_prc -= test_delta_prc;
                break;

            case 'z':   // increase speed wheel B
                test_duty_B_prc += test_delta_prc;
                break;

            case 'x':   // decrease speed wheel B
                test_duty_B_prc -= test_delta_prc;
                break;

            case 'q':   // increase speed wheel C
                test_duty_C_prc += test_delta_prc;
                break;

            case 'w':   // decrease speed wheel C
                test_duty_C_prc -= test_delta_prc;
                break;

            case ' ':   // stop all wheels
                test_duty_A_prc = 0;
                test_duty_B_prc = 0;
                test_duty_C_prc = 0;
                break;

            default:
                break;
        }

        lldControlSetMotorPower( A, test_duty_A_prc );
        lldControlSetMotorPower( B, test_duty_B_prc );
        lldControlSetMotorPower( C, test_duty_C_prc );

        if( matlab_start_flag )
        {
            matlab_wheel_A      = (int)(test_wheel_A * 100);
            matlab_wheel_A_raw  = (int)(test_wheel_A_raw * 100);
            sdWrite( &SD3, (uint8_t*) &matlab_wheel_A, 2 );
            sdWrite( &SD3, (uint8_t*) &matlab_wheel_A_raw, 2 );
        }
        time = chThdSleepUntilWindowed( time, time + MS2ST( 10 ) );
#else
        dbgprintf( "A: %d\tB: %d\tC: %d\n\r",
                   (int)test_wheel_A,
                   (int)test_wheel_B,
                   (int)test_wheel_C );
        time = chThdSleepUntilWindowed( time, time + MS2ST( 300 ) );
#endif

    }
}

/*
 * @brief   Test linear robot speed calculation in Matlab
 */
void testLinearSpeedMatlab( void )
{
    wheelControlInit();

    sdStart( &SD6, &sdcfg );
    palSetPadMode( GPIOG, 14, PAL_MODE_ALTERNATE(8) );   // TX
    palSetPadMode( GPIOG, 9,  PAL_MODE_ALTERNATE(8) );   // RX

    odometrySpeedValue_t    odom_wheel_speed_A          = 0;
    odometrySpeedValue_t    odom_wheel_speed_B          = 0;
    odometrySpeedValue_t    odom_wheel_speed_C          = 0;

    int16_t                 matlab_speed_A              = 0;
    int16_t                 matlab_speed_B              = 0;
    int16_t                 matlab_speed_C              = 0;

    int16_t                 matlab_odom_speed_A         = 0;
    int16_t                 matlab_odom_speed_B         = 0;
    int16_t                 matlab_odom_speed_C         = 0;

    systime_t   time = chVTGetSystemTimeX( );
    while( true )
    {
        sdRead( &SD6, (uint8_t*) &matlab_speed_A, 2 );
        sdRead( &SD6, (uint8_t*) &matlab_speed_B, 2 );
        sdRead( &SD6, (uint8_t*) &matlab_speed_C, 2 );

        if( abs(matlab_speed_A) <= 300 )
        {
            wheelControlSetPermeation();

            float send_speed_A = (float)matlab_speed_A / 100;
            wheelControlSetSpeed(
                send_speed_A, A, REVS_PER_SEC
            );
        }
        if( abs(matlab_speed_B) <= 300 )
        {
            wheelControlSetPermeation();

            float send_speed_B = (float)matlab_speed_B / 100;
            wheelControlSetSpeed(
                send_speed_B, B, REVS_PER_SEC
            );
        }
        if( abs(matlab_speed_C) <= 300 )
        {
            wheelControlSetPermeation();

            float send_speed_C = (float)matlab_speed_C / 100;
            wheelControlSetSpeed(
                send_speed_C, C, REVS_PER_SEC
            );
        }

        odom_wheel_speed_A = odometryGetWheelSpeed( A, REVS_PER_SEC );
        odom_wheel_speed_B = odometryGetWheelSpeed( B, REVS_PER_SEC );
        odom_wheel_speed_C = odometryGetWheelSpeed( C, REVS_PER_SEC );

        if( abs(matlab_speed_A) == 500 || abs(matlab_speed_B) == 500 || abs(matlab_speed_C) == 500 )
        {
            palSetLine( LINE_LED3 );

            wheelControlSetSpeed(
                0, A, REVS_PER_SEC
            );

            wheelControlSetSpeed(
                0, B, REVS_PER_SEC
            );

            wheelControlSetSpeed(
                0, C, REVS_PER_SEC
            );
        }
        else
        {
            matlab_odom_speed_A = (int)(odom_wheel_speed_A * 100);
            matlab_odom_speed_B = (int)(odom_wheel_speed_B * 100);
            matlab_odom_speed_C = (int)(odom_wheel_speed_C * 100);

            sdWrite(&SD6, (uint8_t*) &matlab_odom_speed_A, 2);
            sdWrite(&SD6, (uint8_t*) &matlab_odom_speed_B, 2);
            sdWrite(&SD6, (uint8_t*) &matlab_odom_speed_C, 2);
        }

        time = chThdSleepUntilWindowed( time, time + MS2ST( 10 ) );
    }
}

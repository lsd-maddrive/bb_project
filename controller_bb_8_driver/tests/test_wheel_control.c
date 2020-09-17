#include <tests.h>
#include "wheel_control.h"

static const SerialConfig sdcfg = {
  .speed = 115200,
  .cr1 = 0, .cr2 = 0, .cr3 = 0
};

/*
 * @brief   Test wheel control system with matlab
 */
void testWheelControlMatlab( void )
{
    wheelControlInit();
    debug_stream_init();

    sdStart( &SD6, &sdcfg );
    palSetPadMode( GPIOG, 14, PAL_MODE_ALTERNATE(8) );   // TX
    palSetPadMode( GPIOG, 9,  PAL_MODE_ALTERNATE(8) );   // RX

    odometrySpeedValue_t    test_wheel_speed_A          = 0;
    odometrySpeedValue_t    test_wheel_speed_B          = 0;
    odometrySpeedValue_t    test_wheel_speed_C          = 0;
    // outside of limits to indicate start connection with matlab
    int16_t                 matlab_ref_wheel_speed_RPS  = 500;
    int16_t                 matlab_wheel_speed_A        = 0;
    int16_t                 matlab_wheel_speed_B        = 0;
    int16_t                 matlab_wheel_speed_C        = 0;

    systime_t   time = chVTGetSystemTimeX( );
    while( true )
    {
        sdReadTimeout( &SD6, (uint8_t*) &matlab_ref_wheel_speed_RPS, 2, TIME_IMMEDIATE );


        if( matlab_ref_wheel_speed_RPS <= 300 && matlab_ref_wheel_speed_RPS >= -300)
        {
            float send_speed = (float)matlab_ref_wheel_speed_RPS / 100;
            wheelControlSetPermeation();

            wheelControlSetSpeed(
                send_speed, A, REVS_PER_SEC
            );

            wheelControlSetSpeed(
                send_speed, B, REVS_PER_SEC
            );

            wheelControlSetSpeed(
                send_speed, C, REVS_PER_SEC
            );

            test_wheel_speed_A = odometryGetWheelSpeed( A, REVS_PER_SEC );
            test_wheel_speed_B = odometryGetWheelSpeed( B, REVS_PER_SEC );
            test_wheel_speed_C = odometryGetWheelSpeed( C, REVS_PER_SEC );

            matlab_wheel_speed_A = (int)(test_wheel_speed_A * 100);
            matlab_wheel_speed_B = (int)(test_wheel_speed_B * 100);
            matlab_wheel_speed_C = (int)(test_wheel_speed_C * 100);

            sdWrite(&SD6, (uint8_t*) &matlab_ref_wheel_speed_RPS, 2);
            sdWrite(&SD6, (uint8_t*) &matlab_wheel_speed_A, 2);
            sdWrite(&SD6, (uint8_t*) &matlab_wheel_speed_B, 2);
            sdWrite(&SD6, (uint8_t*) &matlab_wheel_speed_C, 2);

        }
        else
        {
            // hard stop without control system
            wheelControlResetPermeation();
            lldControlSetMotorPower( A, 0 );
            lldControlSetMotorPower( B, 0 );
            lldControlSetMotorPower( C, 0 );
        }

        time = chThdSleepUntilWindowed( time, time + MS2ST( 10 ) );
    }
}


/*
 * @brief   Test wheel control system with matlab
 * @note    Send two packages with float value (4-byte)
 *          only wheel A data are send to matlab
 */
void testOneWheelControllerMatlab( void )
{
    wheelControlInit();
    debug_stream_init();

    sdStart( &SD6, &sdcfg );
    palSetPadMode( GPIOG, 14, PAL_MODE_ALTERNATE(8) );   // TX
    palSetPadMode( GPIOG, 9,  PAL_MODE_ALTERNATE(8) );   // RX

    bool                    matlab_start_flag   = false;

    wheelSpeedValue_t       test_ref_speed      = 0.3f;
    wheelSpeedValue_t       test_real_speed     = 0.0f;

    wheelControlSetPermeation( );

    systime_t   time = chVTGetSystemTimeX( );
    while( true )
    {
        char rc_data = sdGetTimeout( &SD6, TIME_IMMEDIATE );

        switch( rc_data )
        {
            case 's':   // start
                matlab_start_flag = true;
                break;

            case 'n':   // set speed to 0
                test_ref_speed = 0.02f;
                break;

            case 'm':   // set speed to negative
                test_ref_speed = -test_ref_speed;
                break;

            case 'x':   // stop
                matlab_start_flag = false;
                break;

            default:
              break;
        }

        if( matlab_start_flag )
        {
            wheelControlSetPermeation( );
            wheelControlSetSpeedAllWheels( test_ref_speed, REVS_PER_SEC );

            test_real_speed = odometryGetWheelSpeed( A, REVS_PER_SEC );

            sdWrite(&SD6, (uint8_t*) &test_real_speed, 4);
            sdWrite(&SD6, (uint8_t*) &test_ref_speed,  4);
        }
        else
        {
            wheelControlResetPermeation( );
            wheelControlResetControllerAllWheels();
            lldControlStopAllMotors();
        }

        time = chThdSleepUntilWindowed( time, time + MS2ST( 10 ) );
    }
}

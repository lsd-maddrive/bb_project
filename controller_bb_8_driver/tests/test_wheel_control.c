#include <tests.h>
#include "wheel_control.h"

static const SerialConfig sdcfg = {
  .speed = 115200,
  .cr1 = 0, .cr2 = 0, .cr3 = 0
};

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
            palSetLine( LINE_LED3 );
            if( matlab_ref_wheel_speed_RPS == 0 )
            {
                wheelControlResetPermeation();
                lldControlSetMotorPower( A, 0 );
                lldControlSetMotorPower( B, 0 );
                lldControlSetMotorPower( C, 0 );
            }
            else
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
            }

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
            wheelControlResetPermeation();
            lldControlSetMotorPower( A, 0 );
            lldControlSetMotorPower( B, 0 );
            lldControlSetMotorPower( C, 0 );
        }

        time = chThdSleepUntilWindowed( time, time + MS2ST( 10 ) );
    }
}

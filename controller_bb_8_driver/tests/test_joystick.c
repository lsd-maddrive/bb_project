#include <tests.h>

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
        sdReadTimeout( &SD6, (uint8_t*) &buf, 6, TIME_IMMEDIATE );

        dbgprintf("x: %d\ty: %d\tw: %d\n\r",
                  buf[0] * 100, buf[1] * 100, buf[2] * 100);

        time = chThdSleepUntilWindowed( time, time + MS2ST( 100 ) );
    }
}

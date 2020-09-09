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

    int16_t serial_v_x  = 0;
    int16_t serial_v_y  = 0;
    int16_t serial_w    = 0;

    systime_t   time = chVTGetSystemTimeX( );
    while( true )
    {
        sdRead( &SD6, (uint8_t*) &serial_v_x, 2 );
        sdRead( &SD6, (uint8_t*) &serial_v_y, 2 );
        sdRead( &SD6, (uint8_t*) &serial_w, 2 );

        dbgprintf("x: %d\ty: %d\tw: %d\n\r",
                  serial_v_x, serial_v_y, serial_w);

        time = chThdSleepUntilWindowed( time, time + MS2ST( 50 ) );
    }

}

#include <tests.h>
#include <gyroscope.h>

//#define MATLAB_TEST

#ifdef MATLAB_TEST
static const SerialConfig sdcfg = {
  .speed = 115200,
  .cr1 = 0, .cr2 = 0, .cr3 = 0
};
#endif

void testGyroscope(void)
{
    i2cStartUp();

#ifdef MATLAB_TEST
    sdStart( &SD6, &sdcfg );
    palSetPadMode( GPIOG, 14, PAL_MODE_ALTERNATE(8) );   // TX
    palSetPadMode( GPIOG, 9,  PAL_MODE_ALTERNATE(8) );   // RX
#else
    debug_stream_init();
#endif

    gyroscopeInit( NORMALPRIO );
    systime_t time = chVTGetSystemTime();
    while (true)
    {
        float axis = getGyroAngle(GYRO_AXIS_Z);

#ifdef MATLAB_TEST
        sdWrite(&SD6, (uint8_t*) &axis, 4);
        time = chThdSleepUntilWindowed(time, time + MS2ST(50));
#else
        dbgprintf("Z: %d\n\r", (int)(axis));
        time = chThdSleepUntilWindowed(time, time + MS2ST(100));
#endif
    }
}

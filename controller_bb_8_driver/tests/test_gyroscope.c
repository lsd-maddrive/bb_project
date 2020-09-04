#include <tests.h>
#include <gyroscope.h>

static const SerialConfig sdcfg = {
  .speed = 115200,
  .cr1 = 0, .cr2 = 0, .cr3 = 0
};

void testGyroscope(void)
{
    chSysInit();
    halInit();
    debug_stream_init();
    i2cStartUp();

    sdStart( &SD6, &sdcfg );
    palSetPadMode( GPIOG, 14, PAL_MODE_ALTERNATE(8) );   // TX
    palSetPadMode( GPIOG, 9,  PAL_MODE_ALTERNATE(8) );   // RX

    gyroscopeInit();
    systime_t time = chVTGetSystemTime();

    while (true)
    {
        float axis = getGyroAngle(GYRO_AXIS_X);
        sdWrite(&SD6, (uint8_t*) &axis, 4);
        time = chThdSleepUntilWindowed(time, time + MS2ST(50));
    }
}

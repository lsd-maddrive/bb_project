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
        int16_t x_axis = (int)(getGyroAngle(GYRO_AXIS_Z) * 100);
        sdWrite(&SD6, (int8_t*) &x_axis, 2);

        //dbgprintf("X = %d\tY = %d\tZ = %d\n\r", (int)(getGyroAngle(GYRO_AXIS_X) * 100), (int)(getGyroAngle(GYRO_AXIS_Y) * 100), (int)(getGyroAngle(GYRO_AXIS_Z) * 100));

        time = chThdSleepUntilWindowed(time, time + MS2ST(5));

    }
}

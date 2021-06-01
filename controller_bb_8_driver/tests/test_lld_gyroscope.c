#include <tests.h>
#include <lld_gyroscope.h>

#define MATLAB_TEST

#ifdef MATLAB_TEST
static const SerialConfig sdcfg = {
  .speed = 115200,
  .cr1 = 0, .cr2 = 0, .cr3 = 0
};
#endif

void testGyroscope(void)
{
#ifdef MATLAB_TEST
    sdStart( &SD3, &sdcfg );
    palSetPadMode( GPIOG, 14, PAL_MODE_ALTERNATE(8) );   // TX
    palSetPadMode( GPIOG, 9,  PAL_MODE_ALTERNATE(8) );   // RX
#else
    debug_stream_init();
#endif
    float axis[3] = {0, 0, 0};
    uint8_t flag[1] = {15};
    gyroscopeInit( NORMALPRIO );
    systime_t time = chVTGetSystemTime();
    while (true)
    {
        axis[0] = getGyroAngle(GYRO_AXIS_X);
        axis[1] = getGyroAngle(GYRO_AXIS_Y);
        axis[2] = getGyroAngle(GYRO_AXIS_Z);

#ifdef MATLAB_TEST
        sdWrite(&SD3, (uint8_t*) &flag, 1);
        sdWrite(&SD3, (uint8_t*) &axis, 12);
        time = chThdSleepUntilWindowed(time, time + MS2ST(100));
#else
        //dbgprintf("X: %d\tY: %d\tZ: %d\n\r", (int)(getGyroAngle(GYRO_AXIS_X)), (int)(getGyroAngle(GYRO_AXIS_Y)), (int)(getGyroAngle(GYRO_AXIS_Z)));
        dbgprintf("%d%d%d", (int)(getGyroAngle(GYRO_AXIS_X)), (int)(getGyroAngle(GYRO_AXIS_Y)), (int)(getGyroAngle(GYRO_AXIS_Z)));
        time = chThdSleepUntilWindowed(time, time + MS2ST(100));
#endif
    }
}

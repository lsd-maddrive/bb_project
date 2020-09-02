#include <tests.h>
#include <lcd.h>
#include <gyroscope.h>

void testGyroscope(void)
{
    chSysInit();
    halInit();
    i2cStartUp();

    lcdInit();
    lcdClear();
    gyroscopeInit();
    startGyroPosition();
    systime_t time = chVTGetSystemTime();

    while (true)
    {
        lcdClear();
        lcdSendNumber(0, 0, getGyroAngle(0));
        lcdSendNumber(8, 0, getGyroAngle(1));
        lcdSendNumber(0, 1, getGyroAngle(2));


        systime_t prev = time;
        chThdSleepUntilWindowed(prev, time += MS2ST(200));

    }
}

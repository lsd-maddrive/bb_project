#include <tests.h>

static const SerialConfig sdcfg = {
  .speed = 115200,
  .cr1 = 0, .cr2 = 0, .cr3 = 0
};

float package[4] = {0, 0, 0, 0};
int8_t start = 1;
int8_t running = 0;
float buff[3] = {0, 0, 0};

void testLogger(void)
{
    palSetLine(LINE_LED3);
    sdStart( &SD6, &sdcfg );
    palSetPadMode( GPIOG, 14, PAL_MODE_ALTERNATE(8) );   // TX
    palSetPadMode( GPIOG, 9,  PAL_MODE_ALTERNATE(8) );   // RX

    sdWrite(&SD6, (uint8_t*) &start, 1);
    sdWrite(&SD6, (uint8_t*) &package, 16);

    systime_t time = chVTGetSystemTime();
    while (true)
    {
        sdReadTimeout( &SD6, (uint8_t*) &buff, 12, TIME_IMMEDIATE );
        sdWrite(&SD6, (uint8_t*) &running, 1);
        sdWrite(&SD6, (uint8_t*) &package, 16);

        for(uint8_t i = 0; i < 4; i++)
        {
            package[i] += i;
        }
        palToggleLine(LINE_LED2);
        time = chThdSleepUntilWindowed(time, time + MS2ST(100));

    }
}

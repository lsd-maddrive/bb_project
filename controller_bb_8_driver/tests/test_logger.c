#include <tests.h>

static const SerialConfig sdcfg = {
  .speed = 115200,
  .cr1 = 0, .cr2 = 0, .cr3 = 0
};

struct package_t {
    uint8_t logger_key[3];
    int8_t  mode;
    float   data[4];
};

struct package_t log_data = {
     .logger_key = {150, 139, 57},
     .mode = 1,
     .data = {1.0, 2.0, 3.0, 4.0}
};

float buff[3] = {0, 0, 0};

// TODO: add comments!
void testLogger(void)
{
    sdStart( &SD6, &sdcfg );
    palSetPadMode( GPIOG, 14, PAL_MODE_ALTERNATE(8) );   // TX
    palSetPadMode( GPIOG, 9,  PAL_MODE_ALTERNATE(8) );   // RX
    sdWrite(&SD6, (uint8_t*) &log_data, 20);
    log_data.mode = 0;
    chThdSleepMilliseconds(300);

    systime_t time = chVTGetSystemTime();
    while (true)
    {
        sdReadTimeout(&SD6, (uint8_t*) &buff, 12, TIME_IMMEDIATE);
        sdWrite(&SD6, (uint8_t*) &log_data, 20);

        for(uint8_t i = 0; i < 3; i++)
        {
            log_data.data[i] = buff[i];
        }
        time = chThdSleepUntilWindowed(time, time + MS2ST(100));
    }
}

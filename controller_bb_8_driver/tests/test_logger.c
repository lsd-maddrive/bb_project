#include <tests.h>

static const SerialConfig sdcfg = {
  .speed = 115200,
  .cr1 = 0, .cr2 = 0, .cr3 = 0
};

struct package_t {
    int8_t  mode;
    float   data[4];
};

struct package_t log_data = {
     .mode = 1,
     .data = {1.0, 2.0, 3.0, 4.0}
};

float buff[3] = {0, 0, 0};

void testLogger(void)
{
    debug_stream_init();
    sdStart( &SD6, &sdcfg );
    palSetPadMode( GPIOG, 14, PAL_MODE_ALTERNATE(8) );   // TX
    palSetPadMode( GPIOG, 9,  PAL_MODE_ALTERNATE(8) );   // RX

    dbgprintf("%b%f%f%f%f", log_data.mode, log_data.data[0], log_data.data[1], log_data.data[2], log_data.data[3]);
    log_data.mode = 0;
    chThdSleepMilliseconds(300);

    systime_t time = chVTGetSystemTime();
    while (true)
    {
        sdReadTimeout( &SD3, (uint8_t*) &buff, 12, TIME_IMMEDIATE );
        dbgprintf("%b%f%f%f%f", log_data.mode, log_data.data[0], log_data.data[1], log_data.data[2], log_data.data[3]);

        for(uint8_t i = 0; i < 4; i++)
        {
            log_data.data[i] += 1 + i;
        }
//        dbgprintf("I %d\t%d\t%d\n\r", (int)(buff[0]), (int)(buff[1]), (int)(buff[2]));
//        dbgprintf("O %d\t%d\t%d\t%d\t%d\n\r", (int)(log_data.mode),
//                  (int)(log_data.data[0]), (int)(log_data.data[1]),
//                  (int)(log_data.data[2]), (int)(log_data.data[3]));
        time = chThdSleepUntilWindowed(time, time + MS2ST(1000));

    }
}

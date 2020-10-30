#include "logger.h"

struct package_t {
    uint8_t logger_key[3];
    uint8_t  mode;
    float   data[9];
};

// Struct to store data to log
struct package_t logData = {
     .logger_key = {150, 139, 57},
     .mode = 1,
     .data = {0, 0, 0, 0, 0, 0, 0, 0, 0}
};

void sendLog(SerialDriver *sdp,
             float value1,
             float value2,
             float value3,
             float value4,
             float value5,
             float value6,
             float value7,
             float value8,
             float value9)
{
    // Update struct
    logData.data[0] = value1;
    logData.data[1] = value2;
    logData.data[2] = value3;
    logData.data[3] = value4;
    logData.data[4] = value5;
    logData.data[5] = value6;
    logData.data[6] = value7;
    logData.data[7] = value8;
    logData.data[8] = value9;
    sdWrite(sdp, (uint8_t*) &logData, 40);

    // mode = 1 only for first transmit
    logData.mode = 0;
}

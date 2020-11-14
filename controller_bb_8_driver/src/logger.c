#include "logger.h"

struct package_t {
    uint8_t logger_key[3];
    uint8_t  mode;
    float   data[11];
};

// Struct to store data to log
struct package_t logData = {
     .logger_key = {150, 139, 57},
     .mode = 1,
     .data = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

// TODO: add comments!
void sendLog(SerialDriver *sdp, float *log_data, uint8_t log_quantity)
{
    // Update struct
    for(uint8_t i = 0; i < log_quantity; i++)
    {
        logData.data[i] = log_data[i];
    }

    sdWrite(sdp, (uint8_t*) &logData, 48);

    // mode = 1 only for first transmit
    logData.mode = 0;
}

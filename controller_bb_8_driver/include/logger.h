#ifndef LOGGER_H
#define LOGGER_H

#include <common.h>

/*
 * @brief       send log data via USART
 */
void sendLog(SerialDriver *sdp,
             float value1,
             float value2,
             float value3,
             float value4,
             float value5,
             float value6,
             float value7,
             float value8,
             float value9,
             float value10);

#endif

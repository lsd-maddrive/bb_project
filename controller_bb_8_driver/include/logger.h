#ifndef LOGGER_H
#define LOGGER_H

#include <common.h>

/*
 * @brief       send log data via USART
 */
void sendLog(SerialDriver *sdp, float *log_data, uint8_t log_quantity);

#endif

#ifndef INCLUDE_ODOMETRY_H_
#define INCLUDE_ODOMETRY_H_

#include "common.h"
#include "lld_encoder.h"

#define MS_2_SEC        100   // 10 ms -> 1 s
// #define MS_2_SEC     200   // 5 ms  -> 1 s
// #define MS_2_SEC     1000  // 1 ms  -> 1 s
#define CM_2_M          (float)0.01;

#define VT_ODOM_MS      10

typedef float   odometryValue_t;
typedef float   odometrySpeedValue_t;


typedef enum {
    REVS_PER_SEC
} odometrySpeedUnit_t;

/**
 * @brief   Initialize periphery connected to encoder for odometry
 * @note    Stable for repeated calls
 */
void odometryInit( void );

/**
 * @brief   Get speed of rotation of motor
 * @args    Units of speed
 *              [REVS_PER_SEC]    - revolutions per second
 */
odometrySpeedValue_t odometryGetEncoderSpeed ( motorNumberValue_t number, odometrySpeedUnit_t unit );

/**
 * @brief   Get filtered speed of rotation of the wheel
 * @args    Units of speed
 *              [REVS_PER_SEC]    - revolutions per second
 */
odometrySpeedValue_t odometryGetWheelSpeed( motorNumberValue_t motor_name, odometrySpeedUnit_t unit );

/**
 * @brief   Get raw speed of rotation of the wheel
 * @args    Units of speed
 *              [REVS_PER_SEC]    - revolutions per second
 */
odometrySpeedValue_t odometryGetWheelSpeedRaw( motorNumberValue_t number, odometrySpeedUnit_t unit );

#endif

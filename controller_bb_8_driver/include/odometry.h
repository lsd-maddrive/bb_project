#ifndef INCLUDE_ODOMETRY_H_
#define INCLUDE_ODOMETRY_H_

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
float odometryGetEncoderSpeed ( motorNumberValue_t number, odometrySpeedUnit_t unit );

#endif 

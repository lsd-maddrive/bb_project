#ifndef INCLUDE_ROBOT_ODOMETRY_H_
#define INCLUDE_ROBOT_ODOMETRY_H_

#include <common.h>
#include "wheel_control.h"
#include "gyroscope.h"

#define ANGLE_VT_MS     10

/**
 * @brief       Initialization of robot odometry unit
 */
void robotOdometryInit( void );

/**
 * @brief       Set linera speed of robot 
 * @args
 *              v_x - linear speed x-axis [m/s]
 *              v_y - linear speed y-axis [m/s]
 */
void robotOdometrySetSpeed( float v_x_glob, float v_y_glob );

/**
 * @brief       Integral of (input) d_phi
 */
void robotOdometryAddAngle( float angle );

#endif /* INCLUDE_ROBOT_ODOMETRY_H_ */

#ifndef INCLUDE_ROBOT_ODOMETRY_H_
#define INCLUDE_ROBOT_ODOMETRY_H_

#include <common.h>
#include "wheel_control.h"
#include "gyroscope.h"

/**
 * @brief       Initialization of robot odometry unit
 */
void robotOdometryInit( void );

/**
 * @brief       Set linera speed of robot 
 * @args
 *              v_x - linear speed x-axis [m/s]
 *              v_y - linear speed y-axis [m/s]
 *              w   - angular speed [rad/s]
 */
void robotOdometrySetSpeed( float v_x, float v_y, float w );

#endif /* INCLUDE_ROBOT_ODOMETRY_H_ */

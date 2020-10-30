#ifndef INCLUDE_ROBOT_ODOMETRY_H_
#define INCLUDE_ROBOT_ODOMETRY_H_

#include <common.h>
#include "wheel_control.h"
#include "gyroscope.h"

#define ANGLE_VT_MS     10

#define GRAD_2_RAD      (float)(M_PI / 180)

/**
 * @brief       Initialization of robot odometry unit
 */
void robotOdometryInit( void );

/**
 * @brief       Set linera speed of robot 
 * @args
 *              v_x_glob - linear speed x-axis [m/s]
 *              v_y_glob - linear speed y-axis [m/s]
 *              ang_speed- angular speed [rad/s]
 *              k        - time coefficient (convert s to ms)
 *
 */
void robotOdometrySetSpeed( float v_x_glob, float v_y_glob, float angle_glob, float k );

/**
 * @brief       getter for logger purpose
 */
float getSetAngle( void );

/**
 * @brief       getter for logger purpose
 */
float getVelocityXLocal( void );

/**
 * @brief       getter for logger purpose
 */
float getVelocityYLocal( void );

/**
 * @brief       getter for logger purpose
 */
float getWheelASpeed( void );

/**
 * @brief       getter for logger purpose
 */
float getWheelBSpeed( void );

/**
 * @brief       getter for logger purpose
 */
float getWheelCSpeed( void );


#endif /* INCLUDE_ROBOT_ODOMETRY_H_ */

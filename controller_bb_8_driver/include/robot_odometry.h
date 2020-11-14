#ifndef INCLUDE_ROBOT_ODOMETRY_H_
#define INCLUDE_ROBOT_ODOMETRY_H_

#include <common.h>
#include "wheel_control.h"
#include "lld_gyroscope.h"

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
 * @brief      Get value of angle's integral
 * @note       Getter for logger purpose
 */
float robotOdometryGetAngleIntegral( void );

/**
 * @brief      Get value of local robot velocity (x-axle)
 * @note       Getter for logger purpose
 */
float robotOdometryGetVelocityXLocal( void );

/**
 * @brief      Get value of local robot velocity (y-axle)
 * @note       Getter for logger purpose
 */
float robotOdometryGetVelocityYLocal( void );

/**
 * @brief      Get value of wheel A velocity
 * @note       Getter for logger purpose
 */
float robotOdometryGetWheelASpeed( void );

/**
 * @brief      Get value of wheel B velocity
 * @note       Getter for logger purpose
 */
float robotOdometryGetWheelBSpeed( void );

/**
 * @brief      Get value of wheel C velocity
 * @note       Getter for logger purpose
 */
float robotOdometryGetWheelCSpeed( void );


#endif /* INCLUDE_ROBOT_ODOMETRY_H_ */

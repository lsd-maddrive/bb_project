#include "robot_odometry.h"

static virtual_timer_t  angle_vt;

const float TWO_PIR = WHEEL_RADIUS_M * 2 * M_PI;
// Initialization of matrix A
// kinematic matrix A for three motors
float       k_A[3]  = {0, 0, 0};
float       k_B[3]  = {0, 0, 0};
float       k_C[3]  = {0, 0, 0};

/**
 * @brief       Set linera speed of robot 
 * @args
 *              v_x - linear speed x-axis [m/s]
 *              v_y - linear speed y-axis [m/s]
 *              w   - angular speed [rad/s]
 */
void robotOdometrySetSpeed( float v_x, float v_y, float w )
{
    float wheel_speed_A = k_A[0] * v_x +
                          k_A[1] * v_y +
                          k_A[2] * w;

    float wheel_speed_B = k_B[0] * v_x +
                          k_B[1] * v_y +
                          k_B[2] * w;

    float wheel_speed_C = k_C[0] * v_x +
                          k_C[1] * v_y +
                          k_C[2] * w;

    wheelControlSetSpeed( wheel_speed_A, A, REVS_PER_SEC );
    wheelControlSetSpeed( wheel_speed_B, B, REVS_PER_SEC );
    wheelControlSetSpeed( wheel_speed_C, C, REVS_PER_SEC );
}

static bool isInitialized = false;

/**
 * @brief       Initialization of robot odometry unit
 */
void robotOdometryInit( void )
{
    if( isInitialized )
        return;

    // constants calculation
    float ODOM_K  = BASE_WHEEL_DIST_M / TWO_PIR;

    k_A[1] = (-1) / TWO_PIR;
    k_A[2] = ODOM_K;

    k_B[0] = 0.866 / TWO_PIR;
    k_B[1] = 0.5 / TWO_PIR;
    k_B[2] = ODOM_K;

    k_C[0] = (-0.866) / TWO_PIR;
    k_C[1] = 0.5 / TWO_PIR;
    k_C[2] = ODOM_K;
    // *************************************************

    wheelControlInit();

    // wheel control system is enabled
    wheelControlSetPermeation();

    isInitialized = true;
}

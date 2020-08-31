#include "tests.h"
#include "robot_odometry.h"

const float TWO_PIR = WHEEL_RADIUS_M * 2 * M_PI;
// kinematic matrix A for three motors
float       k_A[3]  = {0, 0, 0};
float       k_B[3]  = {0, 0, 0};
float       k_C[3]  = {0, 0, 0};

float       ref_v_x = 0;
float       ref_v_y = 0;
float       ref_w   = 0;

static virtual_timer_t  robot_odom_vt;

static void robot_odom_vt_cb( void *arg )
{
    arg = arg;  // to avoid warnings

    float wheel_speed_A = k_A[0] * ref_v_x +
                          k_A[1] * ref_v_y +
                          k_A[2] * ref_w;

    float wheel_speed_B = k_B[0] * ref_v_x +
                          k_B[1] * ref_v_y +
                          k_B[2] * ref_w;

    float wheel_speed_C = k_C[0] * ref_v_x +
                          k_C[1] * ref_v_y +
                          k_C[2] * ref_w;

    wheelControlSetSpeed( wheel_speed_A, A, REVS_PER_SEC );
    wheelControlSetSpeed( wheel_speed_B, B, REVS_PER_SEC );
    wheelControlSetSpeed( wheel_speed_C, C, REVS_PER_SEC );

    chSysLockFromISR();
    chVTSetI(&robot_odom_vt, MS2ST( VT_PERIOD_MS ), robot_odom_vt_cb, NULL);
    chSysUnlockFromISR();
}

/**
 * @brief       Set reference value of v_x [m/s]
 *              (linear speed x-axis)
 */
void robotOdometrySetVx( float v_x )
{
    ref_v_x = v_x;
}

/**
 * @brief       Set reference value of v_y [m/s]
 *              (linear speed y-axis)
 */
void robotOdometrySetVy( float v_y )
{
    ref_v_y = v_y;
}

/**
 * @brief       Set reference value of w [rad/s]
 *              (angular speed)
 */
void robotOdometrySetW( float w )
{
    ref_w = w;
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

    chVTObjectInit(&robot_odom_vt);
    chVTSet( &robot_odom_vt, MS2ST( VT_PERIOD_MS ), robot_odom_vt_cb, NULL );

    // wheel control system is enabled
    wheelControlSetPermeation();

    isInitialized = true;
}

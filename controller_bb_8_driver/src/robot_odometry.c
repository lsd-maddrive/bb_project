#include "tests.h"
#include "robot_odometry.h"

const float TWO_PIR = WHEEL_RADIUS_M * 2 * M_PI;
const float ODOM_K  = BASE_WHEEL_DIST_M / TWO_PIR;
const float k_A[3] = {0, ((-1) / TWO_PIR), ODOM_K};
//float k_B[3] = {(   0.866 / TWO_PIR ), (  0.5 / TWO_PIR ), ODOM_K};
//float k_C[3] = {( (-0.866) / TWO_PIR), (  0.5 / TWO_PIR ), ODOM_K};

static virtual_timer_t  robot_odom_vt;

static void robot_odom_vt_cb( void *arg )
{
    arg = arg;  // to avoid warnings

}

static bool isInitialized = false;

/**
 * @brief       Initialization of robot odometry unit
 */
void robotOdometryInit( void )
{
    if( isInitialized )
        return;

    wheelControlInit();

    chVTObjectInit(&robot_odom_vt);
    chVTSet( &robot_odom_vt, MS2ST( VT_PERIOD_MS ), robot_odom_vt_cb, NULL );

    // wheel control system is enabled
    wheelControlSetPermeation();

    isInitialized = true;
}

#include "robot_odometry.h"

float   angleIntegral       = 0;
float   angularSpeedControl = 0;
float   angleIntgController = 0;
float   v_x = 0;
float   v_y = 0;
float   wheel_speed_A       = 0;
float   wheel_speed_B       = 0;
float   wheel_speed_C       = 0;

bool    odom_permeation_flag = false;

/**
 * @brief       Integral of (input) d_phi
 */
static void robotOdometryAddAngle( float angle, float k )
{
    angleIntegral += angle * k;

    angleIntegral = fmodf(angleIntegral, 360);

    angleIntegral = angleIntegral < 0 ? angleIntegral + 360 : angleIntegral;
}

pidControllerValue_t    angleController = {
    .kp = 0.05,
    .ki = 0.0009,
    .kd = 0,
    .intgSaturation = 360,
    .propDeadZone = 3,
    .controlDeadZone = 0
};


static virtual_timer_t  angle_vt;

static void angle_vt_cb( void *arg )
{
    arg = arg;

    if( odom_permeation_flag )
    {
        float realAngle_Z = getGyroAngle( GYRO_AXIS_Z );

        float anglePropError = angleIntegral - realAngle_Z;


        if( anglePropError > 180 )
        anglePropError -= 360;
        else if( anglePropError < -180 )
        anglePropError += 360;


        angleIntgController += angleController.ki * anglePropError;
        angleIntgController = CLIP_VALUE(
            angleIntgController,
            -angleController.intgSaturation,
            angleController.intgSaturation
        );

        angularSpeedControl = angleController.kp * anglePropError + angleIntgController;

        if( fabs(anglePropError) <= angleController.propDeadZone )
        {
            angularSpeedControl = 0;
            angleIntgController = 0;
        }
    }

    chSysLockFromISR();
    chVTSetI( &angle_vt, TIME_MS2I( ANGLE_VT_MS ), angle_vt_cb, NULL );
    chSysUnlockFromISR();
}


const float TWO_PIR = WHEEL_RADIUS_M * 2 * M_PI;
// Initialization of matrix A
// kinematic matrix A for three motors
float       k_A[3]  = {0, 0, 0};
float       k_B[3]  = {0, 0, 0};
float       k_C[3]  = {0, 0, 0};


/**
 * @brief       Set linera speed of robot
 * @args
 *              v_x_glob - linear speed x-axis [m/s]
 *              v_y_glob - linear speed y-axis [m/s]
 *              ang_speed- angular speed [rad/s]
 *              k        - time coefficient (convert s to ms)
 *
 */
void robotOdometrySetSpeed( float v_x_glob, float v_y_glob, float angle_glob, float k )
{
    if( odom_permeation_flag )
    {
        robotOdometryAddAngle( angle_glob, k );


        float real_z_angle  = getGyroAngle( GYRO_AXIS_Z );
        float angle_cos     = cosf(real_z_angle * GRAD_2_RAD);
        float angle_sin     = sinf(real_z_angle * GRAD_2_RAD);

        // convert global v_x_glob, v_y_glob to local
        v_x = -(angle_cos * v_x_glob + angle_sin * v_y_glob);

        v_y = -(-angle_sin * v_x_glob + angle_cos * v_y_glob);



        wheel_speed_A = k_A[0] * v_x +
                        k_A[1] * v_y +
                        k_A[2] * angularSpeedControl;

        wheel_speed_B = k_B[0] * v_x +
                        k_B[1] * v_y +
                        k_B[2] * angularSpeedControl;

        wheel_speed_C = k_C[0] * v_x +
                        k_C[1] * v_y +
                        k_C[2] * angularSpeedControl;

        wheelControlSetSpeed( wheel_speed_A, A, REVS_PER_SEC );
        wheelControlSetSpeed( wheel_speed_B, B, REVS_PER_SEC );
        wheelControlSetSpeed( wheel_speed_C, C, REVS_PER_SEC );
    }
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
    gyroscopeInit( NORMALPRIO );

    // wheel control system is enabled
    wheelControlSetPermeation();

    chVTObjectInit(&angle_vt);
    chVTSet( &angle_vt, TIME_MS2I( ANGLE_VT_MS ), angle_vt_cb, NULL );

    isInitialized = true;
}

/**
 * @brief       Permeation is enabled,
 *              control system is enabled
 */
void robotOdometrySetPermeation( void )
{
    odom_permeation_flag = true;
}

/**
 * @brief       Permeation is disabled,
 *              control system is disabled
 */
void robotOdometryResetPermeation( void )
{
    odom_permeation_flag = false;
}

/************ Just bunch of getters ****************/

/**
 * @brief      Get value of angle's integral
 * @note       Getter for logger purpose
 */
float robotOdometryGetAngleIntegral( void )
{
    return angleIntegral;
}

/**
 * @brief      Get value of local robot velocity (x-axle)
 * @note       Getter for logger purpose
 */
float robotOdometryGetVelocityXLocal( void )
{
    return v_x;
}

/**
 * @brief      Get value of local robot velocity (y-axle)
 * @note       Getter for logger purpose
 */
float robotOdometryGetVelocityYLocal()
{
    return v_y;
}

// TODO: create function with args as wheel name
//       one function instead of three

/**
 * @brief      Get value of wheel A velocity
 * @note       Getter for logger purpose
 */
float robotOdometryGetWheelASpeed()
{
    return wheel_speed_A;
}

/**
 * @brief      Get value of wheel B velocity
 * @note       Getter for logger purpose
 */
float robotOdometryGetWheelBSpeed()
{
    return wheel_speed_B;
}

/**
 * @brief      Get value of wheel C velocity
 * @note       Getter for logger purpose
 */
float robotOdometryGetWheelCSpeed()
{
    return wheel_speed_C;
}

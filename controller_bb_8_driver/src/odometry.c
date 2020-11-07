#include <odometry.h>

static virtual_timer_t  odom_update_vt;

/*============================================================================*/
/* Variable CONFIGURATION                                                     */
/*============================================================================*/

odometrySpeedValue_t    odometryEncRevPerSec[3]         = {0, 0, 0};
odometrySpeedValue_t    odometryWheelSpeedRPS_LPF[3]    = {0, 0, 0};

/**************** ENCODER 1 ********************/
encoderRevsValue_t      encPrevRevsNumberA      = 0;
encoderRevsValue_t      encCurRevsNumberA       = 0;

odometrySpeedValue_t    odometryEncRevPerSecA   = 0;

#define ENC_A_LPF       (float)0.85
odometrySpeedValue_t    odometryWheelSpeedRPS_A_LPF         = 0;
odometrySpeedValue_t    odometryWheelPrevSpeedRPS_A_LPF     = 0;

/**************** ENCODER 2 ********************/
encoderRevsValue_t      encPrevRevsNumberB      = 0;
encoderRevsValue_t      encCurRevsNumberB       = 0;

odometrySpeedValue_t    odometryEncRevPerSecB   = 0;

#define ENC_B_LPF       (float)0.85
odometrySpeedValue_t    odometryWheelSpeedRPS_B_LPF         = 0;
odometrySpeedValue_t    odometryWheelPrevSpeedRPS_B_LPF     = 0;

/**************** ENCODER 3 ********************/
encoderRevsValue_t      encPrevRevsNumberC      = 0;
encoderRevsValue_t      encCurRevsNumberC       = 0;

odometrySpeedValue_t    odometryEncRevPerSecC   = 0;

#define ENC_C_LPF       (float)0.85
odometrySpeedValue_t    odometryWheelSpeedRPS_C_LPF         = 0;
odometrySpeedValue_t    odometryWheelPrevSpeedRPS_C_LPF     = 0;


static void odom_update_vt_cb( void *arg )
{
    arg = arg; 
/**************** ENCODER A ********************/
    encCurRevsNumberA       = lldGetEncoderRevs( A );
    odometryEncRevPerSec[0] = (encCurRevsNumberA - encPrevRevsNumberA) * MS_2_SEC;
    encPrevRevsNumberA      = encCurRevsNumberA;
// LPF Filter
    odometryWheelSpeedRPS_LPF[0]    = (odometryEncRevPerSec[0] / MOTOR_GAIN) * (1 - ENC_A_LPF) + odometryWheelPrevSpeedRPS_A_LPF * ENC_A_LPF;
    odometryWheelPrevSpeedRPS_A_LPF = odometryWheelSpeedRPS_LPF[0];

/**************** ENCODER B ********************/
    encCurRevsNumberB       = lldGetEncoderRevs( B );
    odometryEncRevPerSec[1] = (encCurRevsNumberB - encPrevRevsNumberB) * MS_2_SEC;
    encPrevRevsNumberB      = encCurRevsNumberB;



// LPF Filter
    odometryWheelSpeedRPS_LPF[1]     = (odometryEncRevPerSec[1] / MOTOR_GAIN) * (1 - ENC_B_LPF) + odometryWheelPrevSpeedRPS_B_LPF * ENC_B_LPF;
    odometryWheelPrevSpeedRPS_B_LPF  = odometryWheelSpeedRPS_LPF[1];

/**************** ENCODER C ********************/
    encCurRevsNumberC       = lldGetEncoderRevs( C );
    odometryEncRevPerSec[2] = (encCurRevsNumberC - encPrevRevsNumberC) * MS_2_SEC;
    encPrevRevsNumberC      = encCurRevsNumberC;
// LPF Filter
    odometryWheelSpeedRPS_LPF[2]     = (odometryEncRevPerSec[2] / MOTOR_GAIN) * (1 - ENC_C_LPF) + odometryWheelPrevSpeedRPS_C_LPF * ENC_C_LPF;
    odometryWheelPrevSpeedRPS_C_LPF  = odometryWheelSpeedRPS_LPF[2];

    chSysLockFromISR();
    chVTSetI(&odom_update_vt, MS2ST( VT_ODOM_MS ), odom_update_vt_cb, NULL);
    chSysUnlockFromISR();
}

static bool isInitialized   = false; 

/**
 * @brief   Initialize periphery connected to encoder for odometry
 * @note    Stable for repeated calls
 */
void odometryInit( void )
{
    if( isInitialized )
        return; 

    chVTObjectInit(&odom_update_vt);
    chVTSet( &odom_update_vt, MS2ST( VT_ODOM_MS ), odom_update_vt_cb, NULL );

    lldEncoderInit( ); 

    isInitialized = true; 
}

/**
 * @brief   Get speed of rotation of motor 
 * @args    Units of speed 
 *              [REVS_PER_SEC]    - revolutions per second 
 */
odometrySpeedValue_t odometryGetEncoderSpeed ( motorNumberValue_t number, odometrySpeedUnit_t unit )
{
    switch( unit )
    {
        case REVS_PER_SEC:
            return odometryEncRevPerSec[number];
            break; // does it make sense after return?

        default:
            return -1.0;
            break; // does it make sense after return?
    }
}

/**
 * @brief   Get filtered speed of rotation of the wheel
 * @args    Units of speed
 *              [REVS_PER_SEC]    - revolutions per second
 */
odometrySpeedValue_t odometryGetWheelSpeed( motorNumberValue_t motor_name, odometrySpeedUnit_t unit )
{
     switch( unit )
     {
         case REVS_PER_SEC:
            return odometryWheelSpeedRPS_LPF[motor_name];
            break;

        default:
            return -1.0;
            break;
     }
}

/**
 * @brief   Get raw speed of rotation of the wheel
 * @args    Units of speed
 *              [REVS_PER_SEC]    - revolutions per second
 */
odometrySpeedValue_t odometryGetWheelSpeedRaw( motorNumberValue_t number, odometrySpeedUnit_t unit )
{
    switch( unit )
    {
        case REVS_PER_SEC:
            return odometryEncRevPerSec[number] / MOTOR_GAIN;
            break;

        default:
            return -1.0;
            break;
    }
}

#include <tests.h>
#include <lld_encoder.h>
#include <odometry.h>


static virtual_timer_t  odom_update_vt;

/*============================================================================*/
/* Variable CONFIGURATION                                                     */
/*============================================================================*/

/**************** ENCODER 1 ********************/
encoderRevsValue_t      encPrevRevsNumberA      = 0;
encoderRevsValue_t      encCurRevsNumberA       = 0;

odometrySpeedValue_t    odometryEncRevPerSecA   = 0;

#define ENC_A_LPF       (float)0.7
odometrySpeedValue_t    odometryWheelSpeedRPS_A_LPF         = 0;
odometrySpeedValue_t    odometryWheelPrevSpeedRPS_A_LPF     = 0;

/**************** ENCODER 2 ********************/
encoderRevsValue_t      encPrevRevsNumberB      = 0;
encoderRevsValue_t      encCurRevsNumberB       = 0;

odometrySpeedValue_t    odometryEncRevPerSecB   = 0;

#define ENC_B_LPF       (float)0.7
odometrySpeedValue_t    odometryWheelSpeedRPS_B_LPF         = 0;
odometrySpeedValue_t    odometryWheelPrevSpeedRPS_B_LPF     = 0;

/**************** ENCODER 3 ********************/
encoderRevsValue_t      encPrevRevsNumberC      = 0;
encoderRevsValue_t      encCurRevsNumberC       = 0;

odometrySpeedValue_t    odometryEncRevPerSecC   = 0;

#define ENC_C_LPF       (float)0.7
odometrySpeedValue_t    odometryWheelSpeedRPS_C_LPF         = 0;
odometrySpeedValue_t    odometryWheelPrevSpeedRPS_C_LPF     = 0;

static void odom_update_vt_cb( void *arg )
{
    arg = arg; 
/**************** ENCODER A ********************/
    encCurRevsNumberA    = lldGetEncoderRevs( A );
    odometryEncRevPerSecA    = (encCurRevsNumberA - encPrevRevsNumberA) * MS_2_SEC;
    encPrevRevsNumberA   = encCurRevsNumberA;
// LPF Filter
    odometryWheelSpeedRPS_A_LPF      = (odometryEncRevPerSecA / MOTOR_GAIN) * (1 - ENC_A_LPF) + odometryWheelPrevSpeedRPS_A_LPF * ENC_A_LPF;
    odometryWheelPrevSpeedRPS_A_LPF  = odometryWheelSpeedRPS_A_LPF;

/**************** ENCODER B ********************/
    encCurRevsNumberB    = lldGetEncoderRevs( B );
    odometryEncRevPerSecB    = (encCurRevsNumberB - encPrevRevsNumberB) * MS_2_SEC;
    encPrevRevsNumberB   = encCurRevsNumberB;
// LPF Filter
    odometryWheelSpeedRPS_B_LPF      = (odometryEncRevPerSecB / MOTOR_GAIN) * (1 - ENC_B_LPF) + odometryWheelPrevSpeedRPS_B_LPF * ENC_B_LPF;
    odometryWheelPrevSpeedRPS_B_LPF  = odometryWheelSpeedRPS_B_LPF;

/**************** ENCODER C ********************/
    encCurRevsNumberC    = lldGetEncoderRevs( C );
    odometryEncRevPerSecC    = (encCurRevsNumberC - encPrevRevsNumberC) * MS_2_SEC;
    encPrevRevsNumberC   = encCurRevsNumberC;
// LPF Filter
    odometryWheelSpeedRPS_C_LPF      = (odometryEncRevPerSecC / MOTOR_GAIN) * (1 - ENC_C_LPF) + odometryWheelPrevSpeedRPS_C_LPF * ENC_C_LPF;
    odometryWheelPrevSpeedRPS_C_LPF  = odometryWheelSpeedRPS_C_LPF;

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
//  number = CLIP_VALUE( number, 0, 2 );

  switch( number )
  {
      case A:
        switch( unit )
        {
            case REVS_PER_SEC:
                return odometryEncRevPerSecA;
                break; // does it make sense after return?

            default:
                return -1.0;
                break; // does it make sense after return?
        }
        break;

      case B:
        switch( unit )
        {
            case REVS_PER_SEC:
                return odometryEncRevPerSecB;
                break; // does it make sense after return?

            default:
                return -1.0;
                break; // does it make sense after return?
        }
        break;

      case C:
        switch( unit )
        {
            case REVS_PER_SEC:
                return odometryEncRevPerSecC;
                break; // does it make sense after return?

            default:
                return -1.0;
                break; // does it make sense after return?
        }
        break;

      default:
        return -1.0;    // useless actually
        break;
  }
}

/**
 * @brief   Get filtered speed of rotation of the wheel
 * @args    Units of speed
 *              [REVS_PER_SEC]    - revolutions per second
 */
odometrySpeedValue_t odometryGetWheelSpeed( motorNumberValue_t number, odometrySpeedUnit_t unit )
{
//    number = CLIP_VALUE( number, 0, 2 );

    switch( number )
    {
        case A:
          switch( unit )
          {
              case REVS_PER_SEC:
                  return odometryWheelSpeedRPS_A_LPF;
                  break;

              default:
                  return -1.0;
                  break;
          }
          break;

        case B:
          switch( unit )
          {
              case REVS_PER_SEC:
                  return odometryWheelSpeedRPS_B_LPF;
                  break;

              default:
                  return -1.0;
                  break;
          }
          break;

        case C:
          switch( unit )
          {
              case REVS_PER_SEC:
                  return odometryWheelSpeedRPS_C_LPF;
                  break;

              default:
                  return -1.0;
                  break;
          }
          break;

        default:
          return -1.0;    // useless actually
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
//    number = CLIP_VALUE( number, 0, 2 );

    switch( number )
    {
        case A:
          switch( unit )
          {
              case REVS_PER_SEC:
                  return odometryEncRevPerSecA / MOTOR_GAIN;
                  break;

              default:
                  return -1.0;
                  break;
          }
          break;

        case B:
          switch( unit )
          {
              case REVS_PER_SEC:
                  return odometryEncRevPerSecB / MOTOR_GAIN;
                  break;

              default:
                  return -1.0;
                  break;
          }
          break;

        case C:
          switch( unit )
          {
              case REVS_PER_SEC:
                  return odometryEncRevPerSecC / MOTOR_GAIN;
                  break;

              default:
                  return -1.0;
                  break;
          }
          break;

        default:
          return -1.0;    // useless actually
          break;
    }
}

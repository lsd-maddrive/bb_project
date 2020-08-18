#include <tests.h>
#include <lld_encoder.h>
#include <odometry.h>

#define MS_2_SEC        100 // 10 ms - > 1 s

#define VT_ODOM_MS      10
static virtual_timer_t  odom_update_vt;

/*============================================================================*/
/* Variable CONFIGURATION                                                     */
/*============================================================================*/

/**************** ENCODER 1 ********************/
float   encPreviousRevsNumber1   = 0; 
float   encCurrentRevsNumber1    = 0; 

float   odometryEnc1RevPerSec    = 0; 

/**************** ENCODER 2 ********************/
float   encPreviousRevsNumber2   = 0;
float   encCurrentRevsNumber2    = 0;

float   odometryEnc2RevPerSec    = 0;

/**************** ENCODER 3 ********************/
float   encPreviousRevsNumber3   = 0;
float   encCurrentRevsNumber3    = 0;

float   odometryEnc3RevPerSec    = 0;


static void odom_update_vt_cb( void *arg )
{
    arg = arg; 
/**************** ENCODER 1 ********************/
    encCurrentRevsNumber1    = lldGetEncoderRevs( 1 );
    odometryEnc1RevPerSec    = (encCurrentRevsNumber1 - encPreviousRevsNumber1) * MS_2_SEC;
    encPreviousRevsNumber1   = encCurrentRevsNumber1;

/**************** ENCODER 2 ********************/
    encCurrentRevsNumber2    = lldGetEncoderRevs( 2 );
    odometryEnc2RevPerSec    = (encCurrentRevsNumber2 - encPreviousRevsNumber2) * MS_2_SEC;
    encPreviousRevsNumber2   = encCurrentRevsNumber2;

/**************** ENCODER 3 ********************/
    encCurrentRevsNumber3    = lldGetEncoderRevs( 3 );
    odometryEnc3RevPerSec    = (encCurrentRevsNumber3 - encPreviousRevsNumber3) * MS_2_SEC;
    encPreviousRevsNumber3   = encCurrentRevsNumber3;

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
float odometryGetEncoderSpeed ( motorNumberValue_t number, odometrySpeedUnit_t unit )
{
  number = CLIP_VALUE( number, 1, 3 );

  switch( number )
  {
      case 1:
        switch( unit )
        {
            case REVS_PER_SEC:
                return odometryEnc1RevPerSec;
                break;

            default:
                return -1.0;
                break;
        }
        break;

      case 2:
        switch( unit )
        {
            case REVS_PER_SEC:
                return odometryEnc2RevPerSec;
                break;

            default:
                return -1.0;
                break;
        }
        break;

      case 3:
        switch( unit )
        {
            case REVS_PER_SEC:
                return odometryEnc3RevPerSec;
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

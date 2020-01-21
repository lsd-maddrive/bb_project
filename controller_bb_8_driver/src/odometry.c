#include <tests.h>
#include <lld_encoder.h>
#include <odometry.h>

#define MS_2_SEC        100 // 10 ms - > 1 s

#define VT_ODOM_MS      10
static virtual_timer_t  odom_update_vt;

/*============================================================================*/
/* Variable CONFIGURATION                                                     */
/*============================================================================*/

float   encoderPreviousRevsNumber   = 0; 
float   encoderCurrentRevsNumber    = 0; 

float   odometryEncoderRevPerSec    = 0; 

static void odom_update_vt_cb( void *arg )
{
    arg = arg; 

    encoderCurrentRevsNumber    = lldGetEncoderRevs( ); 

    odometryEncoderRevPerSec    = (encoderCurrentRevsNumber - encoderPreviousRevsNumber) * MS_2_SEC; 

    encoderPreviousRevsNumber   = encoderCurrentRevsNumber; 
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
float odometryGetEncoderSpeed ( odometrySpeedUnit_t unit )
{
    switch( unit )
    {
        case REVS_PER_SEC: 
            return odometryEncoderRevPerSec; 
            break; 
        
        default:
            return -1.0;
            break; 
    }
}
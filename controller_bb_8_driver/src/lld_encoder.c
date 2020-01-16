#include <tests.h>
#include <lld_encoder.h>

#define ENC_MAX_TICK_NUM        1000

/*============================================================================*/
/* LINE CONFIGURATION                                                         */
/*============================================================================*/

#define ENCODER_GREEN_LINE  PAL_LINE( GPIOD, 5 )
#define ENCODER_WHITE_LINE  PAL_LINE( GPIOD, 4 )

/*============================================================================*/
/* Variable CONFIGURATION                                                     */
/*============================================================================*/

encoderTicksValue_t     enc_tick_cntr   = 0; 
encoderRevsValue_t      enc_revs_cntr   = 0; 
bool                    enc_dir_state   = 0;


/*============================================================================*/
/* Base channel processing                                                    */
/*============================================================================*/
static void extcb_base(EXTDriver *extp, expchannel_t channel)
{
    (void)extp;
    (void)channel;

    /***    To define direction of encoder rotation  ***/
    if( palReadLine( ENCODER_WHITE_LINE ) == 0 )
    {
        enc_tick_cntr    -= 1;
        enc_dir_state    = 1;       // counterclockwise
    }
    else
    {
        enc_tick_cntr    += 1;
        enc_dir_state    = 0;       // clockwise
    }

    /***    Reset counter when it reaches the MAX value  ***/
    /***    Count encoder revolutions                    ***/
    if( enc_tick_cntr == (ENC_MAX_TICK_NUM - 1) )
    {
        enc_revs_cntr   += 1;
        enc_tick_cntr    = 0;
    }
    else if( enc_tick_cntr == (-ENC_MAX_TICK_NUM + 1) )
    {
        enc_revs_cntr   -= 1;
        enc_tick_cntr    = 0;
    }
}


static bool         isInitialized       = false;

/**
 * @brief   Initialize periphery connected to encoder
 * @note    Stable for repeated calls
 */
void lldEncoderInit( void )
{
    if( isInitialized )
        return; 

    EXTChannelConfig base_conf = {
        .mode = EXT_CH_MODE_RISING_EDGE | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOD,
        .cb = extcb_base
    };
    
    static EXTConfig extcfg;
    for ( expchannel_t ch = 0; ch < EXT_MAX_CHANNELS; ch++ )
    {
        extcfg.channels[ch].mode  = EXT_CH_MODE_DISABLED;
        extcfg.channels[ch].cb    = NULL;
    }
    extStart( &EXTD1, &extcfg );
    extSetChannelMode( &EXTD1, 5, &base_conf );

    isInitialized = true;
}

/**
 * @brief   Get number of encoder ticks
 * @note    Max number of ticks is defined by MAX_TICK_NUM
 * @return  Encoder ticks number depends on direction of rotation
 *          [0; ENC_MAX_TICK_NUM]
 *          after ENC_MAX_TICK_NUM it resets
 */
encoderTicksValue_t lldGetEncoderTicks( void )
{
    return enc_tick_cntr;
}

/**
 * @brief   Get direction of encoder rotation
 * @return  clockwise           -> 0
 *          counterclockwise    -> 1
 */
bool lldGetEncoderDirection( void )
{
    return enc_dir_state;
}

/**
 * @brief   Get number of encoder revolutions [float]
 * @note    1 revolution = MAX_TICK_NUM ticks
 * @return  Encoder revolutions number depends on direction of rotation
 */
encoderRevsValue_t lldGetEncoderRevs( void )
{
    return ( enc_revs_cntr + enc_tick_cntr / (float)ENC_MAX_TICK_NUM );
}

/*
 * @brief   Reset all variable for lld-encoder unit
 */
void lldEncoderReset( void )
{
    enc_tick_cntr       = 0;
    enc_revs_cntr       = 0;
    enc_dir_state       = 0;
}
#include <lld_encoder.h>

#define ENC_MAX_TICK_NUM        1000

/*============================================================================*/
/* LINE CONFIGURATION                                                         */
/*============================================================================*/

/**************** MOTOR 1 ********************/
#define ENC1_GREEN_LINE  PAL_LINE( GPIOD, 5 )
#define ENC1_WHITE_LINE  PAL_LINE( GPIOD, 4 )
#define ENC1_BASE        5

/**************** MOTOR 2 ********************/
#define ENC2_GREEN_LINE  PAL_LINE( GPIOD, 7 )
#define ENC2_WHITE_LINE  PAL_LINE( GPIOD, 6 )
#define ENC2_BASE        7

/**************** MOTOR 3 ********************/
#define ENC3_GREEN_LINE  PAL_LINE( GPIOD, 3 )
#define ENC3_WHITE_LINE  PAL_LINE( GPIOE, 2 )
#define ENC3_BASE        3

/*============================================================================*/
/* Variable CONFIGURATION                                                     */
/*============================================================================*/

/**************** MOTOR 1 ********************/
encoderTicksValue_t     enc1_tick_cntr   = 0;
encoderRevsValue_t      enc1_revs_cntr   = 0;
bool                    enc1_dir_state   = 0;

/**************** MOTOR 2 ********************/
encoderTicksValue_t     enc2_tick_cntr   = 0;
encoderRevsValue_t      enc2_revs_cntr   = 0;
bool                    enc2_dir_state   = 0;

/**************** MOTOR 3 ********************/
encoderTicksValue_t     enc3_tick_cntr   = 0;
encoderRevsValue_t      enc3_revs_cntr   = 0;
bool                    enc3_dir_state   = 0;


/*============================================================================*/
/* Base channel processing                                                    */
/*============================================================================*/

// Motor 1
static void extcb_base1(EXTDriver *extp, expchannel_t channel)
{
    (void)extp;
    (void)channel;

    /***    To define direction of encoder rotation  ***/
    if( palReadLine( ENC1_WHITE_LINE ) == 0 )
    {
        enc1_tick_cntr    += 1;
        enc1_dir_state    = 1;       // counterclockwise
    }
    else
    {
        enc1_tick_cntr    -= 1;
        enc1_dir_state    = 0;       // clockwise
    }

    /***    Reset counter when it reaches the MAX value  ***/
    /***    Count encoder revolutions                    ***/
    if( enc1_tick_cntr == (ENC_MAX_TICK_NUM - 1) )
    {
        enc1_revs_cntr   += 1;
        enc1_tick_cntr    = 0;
    }
    else if( enc1_tick_cntr == (-ENC_MAX_TICK_NUM + 1) )
    {
        enc1_revs_cntr   -= 1;
        enc1_tick_cntr    = 0;
    }
}

// Motor 2
static void extcb_base2(EXTDriver *extp, expchannel_t channel)
{
    (void)extp;
    (void)channel;

    /***    To define direction of encoder rotation  ***/
    if( palReadLine( ENC2_WHITE_LINE ) == 0 )
    {
        enc2_tick_cntr    += 1;
        enc2_dir_state    = 1;       // counterclockwise
    }
    else
    {
        enc2_tick_cntr    -= 1;
        enc2_dir_state    = 0;       // clockwise
    }

    /***    Reset counter when it reaches the MAX value  ***/
    /***    Count encoder revolutions                    ***/
    if( enc2_tick_cntr == (ENC_MAX_TICK_NUM - 1) )
    {
        enc2_revs_cntr   += 1;
        enc2_tick_cntr    = 0;
    }
    else if( enc2_tick_cntr == (-ENC_MAX_TICK_NUM + 1) )
    {
        enc2_revs_cntr   -= 1;
        enc2_tick_cntr    = 0;
    }
}

// Motor 3
static void extcb_base3(EXTDriver *extp, expchannel_t channel)
{
    (void)extp;
    (void)channel;

    /***    To define direction of encoder rotation  ***/
    if( palReadLine( ENC3_WHITE_LINE ) == 0 )
    {
        enc3_tick_cntr    += 1;
        enc3_dir_state    = 1;       // counterclockwise
    }
    else
    {
        enc3_tick_cntr    -= 1;
        enc3_dir_state    = 0;       // clockwise
    }

    /***    Reset counter when it reaches the MAX value  ***/
    /***    Count encoder revolutions                    ***/
    if( enc3_tick_cntr == (ENC_MAX_TICK_NUM - 1) )
    {
        enc3_revs_cntr   += 1;
        enc3_tick_cntr    = 0;
    }
    else if( enc3_tick_cntr == (-ENC_MAX_TICK_NUM + 1) )
    {
        enc3_revs_cntr   -= 1;
        enc3_tick_cntr    = 0;
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

    commonExtDriverInit();

    // Motor 1
    EXTChannelConfig base1_conf = {
        .mode = EXT_CH_MODE_RISING_EDGE | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOD,
        .cb = extcb_base1
    };
    extSetChannelMode( &EXTD1, ENC1_BASE, &base1_conf );

    // Motor 2
    EXTChannelConfig base2_conf = {
        .mode = EXT_CH_MODE_RISING_EDGE | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOD,
        .cb = extcb_base2
    };
    extSetChannelMode( &EXTD1, ENC2_BASE, &base2_conf );

    // Motor 3
    EXTChannelConfig base3_conf = {
        .mode = EXT_CH_MODE_RISING_EDGE | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOD,
        .cb = extcb_base3
    };
    extSetChannelMode( &EXTD1, ENC3_BASE, &base3_conf );

    isInitialized = true;
}

/**
 * @brief   Get number of encoder ticks
 * @note    Max number of ticks is defined by MAX_TICK_NUM
 * @return  Encoder ticks number depends on direction of rotation
 *          [0; ENC_MAX_TICK_NUM]
 *          after ENC_MAX_TICK_NUM it resets
 */
encoderTicksValue_t lldGetEncoderTicks( motorNumberValue_t number )
{
    if( number == A )
      return enc1_tick_cntr;
    else if( number == B )
      return enc2_tick_cntr;
    else if( number == C )
      return enc3_tick_cntr;

    // must be Impossible
    return 0;
}

/**
 * @brief   Get direction of encoder rotation
 * @return  clockwise           -> 0
 *          counterclockwise    -> 1
 */
bool lldGetEncoderDirection( motorNumberValue_t number )
{
    if( number == A )
      return enc1_dir_state;
    else if( number == B)
      return enc2_dir_state;
    else if( number == C )
      return enc3_dir_state;

    // must be Impossible
    return 0;
}

/**
 * @brief   Get number of encoder revolutions [float]
 * @note    1 revolution = MAX_TICK_NUM ticks
 * @return  Encoder revolutions number depends on direction of rotation
 */
encoderRevsValue_t lldGetEncoderRevs( motorNumberValue_t number )
{
    if( number == A )
      return ( enc1_revs_cntr + enc1_tick_cntr / (float)ENC_MAX_TICK_NUM );
    else if( number == B )
      return ( enc2_revs_cntr + enc2_tick_cntr / (float)ENC_MAX_TICK_NUM );
    else if( number == C )
      return ( enc3_revs_cntr + enc3_tick_cntr / (float)ENC_MAX_TICK_NUM );

    // must be Impossible
    return 0;
}

/*
 * @brief   Reset all variable for lld-encoder unit
 */
void lldEncoderReset( motorNumberValue_t number )
{
    if( number == A )
    {
        enc1_tick_cntr       = 0;
        enc1_revs_cntr       = 0;
        enc1_dir_state       = 0;
    }
    else if( number == B )
    {
        enc2_tick_cntr       = 0;
        enc2_revs_cntr       = 0;
        enc2_dir_state       = 0;
    }
    else if( number == C )
    {
        enc3_tick_cntr       = 0;
        enc3_revs_cntr       = 0;
        enc3_dir_state       = 0;
    }
}

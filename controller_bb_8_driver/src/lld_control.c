#include <tests.h>
#include <lld_control.h> 

/*============================================================================*/
/* PWM CONFIGURATION PINS                                                     */
/*============================================================================*/

/**************** MOTOR 1 ********************/
/**************** PWM1 + PWM4 ****************/

#define MOTOR1_PWM4CH_HIN1      3
#define MOTOR1_PD15_ACTIVE      PWM_OUTPUT_ACTIVE_HIGH
#define MOTOR1_PD15_DISABLE     PWM_OUTPUT_DISABLED
#define MOTOR1_PWM_LINE_HIN1    PAL_LINE(GPIOD, 15)

#define MOTOR1_PWM4CH_LIN1      2
#define MOTOR1_PD14_ACTIVE      PWM_OUTPUT_ACTIVE_HIGH
#define MOTOR1_PD14_DISABLE     PWM_OUTPUT_DISABLED
#define MOTOR1_PWM_LINE_LIN1    PAL_LINE(GPIOD, 14)

#define MOTOR1_PWM1CH_HIN2      1
#define MOTOR1_PE11_ACTIVE      PWM_OUTPUT_ACTIVE_HIGH
#define MOTOR1_PE11_DIASBLE     PWM_OUTPUT_DISABLED
#define MOTOR1_PWM_LINE_HIN2    PAL_LINE(GPIOE, 11)

#define MOTOR1_PWM1CH_LIN2      2
#define MOTOR1_PE13_ACTIVE      PWM_OUTPUT_ACTIVE_HIGH
#define MOTOR1_PE13_DISABLE     PWM_OUTPUT_DISABLED
#define MOTOR1_PWM_LINE_LIN2    PAL_LINE(GPIOE, 13)

/**************** MOTOR 2 ********************/
/**************** PWM1 + PWM2 ****************/

#define MOTOR2_PWM2CH_HIN1      2
#define MOTOR2_PB10_ACTIVE      PWM_OUTPUT_ACTIVE_HIGH
#define MOTOR2_PB10_DISABLE     PWM_OUTPUT_DISABLED
#define MOTOR2_PWM_LINE_HIN1    PAL_LINE(GPIOB, 10)

#define MOTOR2_PWM2CH_LIN1      0
#define MOTOR2_PA0_ACTIVE      PWM_OUTPUT_ACTIVE_HIGH
#define MOTOR2_PA0_DISABLE     PWM_OUTPUT_DISABLED
#define MOTOR2_PWM_LINE_LIN1    PAL_LINE(GPIOA, 0)

#define MOTOR2_PWM2CH_HIN2      3
#define MOTOR2_PB11_ACTIVE      PWM_OUTPUT_ACTIVE_HIGH
#define MOTOR2_PB11_DISABLE     PWM_OUTPUT_DISABLED
#define MOTOR2_PWM_LINE_HIN2    PAL_LINE(GPIOB, 11)

#define MOTOR2_PWM1CH_LIN2      3
#define MOTOR2_PE14_ACTIVE      PWM_OUTPUT_ACTIVE_HIGH
#define MOTOR2_PE14_DISABLE     PWM_OUTPUT_DISABLED
#define MOTOR2_PWM_LINE_LIN2    PAL_LINE(GPIOE, 14)

/**************** MOTOR 3 ********************/
/**************** PWM2 + PWM3 + PWM4 *********/

#define MOTOR3_PWM3CH_HIN1      1
#define MOTOR3_PC7_ACTIVE       PWM_OUTPUT_ACTIVE_HIGH
#define MOTOR3_PC7_DISABLE      PWM_OUTPUT_DISABLED
#define MOTOR3_PWM_LINE_HIN1    PAL_LINE(GPIOC, 7)

#define MOTOR3_PWM4CH_LIN1      0
#define MOTOR3_PB6_ACTIVE       PWM_OUTPUT_ACTIVE_HIGH
#define MOTOR3_PB6_DISABLE      PWM_OUTPUT_DISABLED
#define MOTOR3_PWM_LINE_LIN1    PAL_LINE(GPIOB, 6)

#define MOTOR3_PWM3CH_HIN2      0
#define MOTOR3_PC6_ACTIVE       PWM_OUTPUT_ACTIVE_HIGH
#define MOTOR3_PC6_DISABLE      PWM_OUTPUT_DISABLED
#define MOTOR3_PWM_LINE_HIN2    PAL_LINE(GPIOC, 6)

#define MOTOR3_PWM2CH_LIN2      1
#define MOTOR3_PB3_ACTIVE       PWM_OUTPUT_ACTIVE_HIGH
#define MOTOR3_PB3_DISABLE      PWM_OUTPUT_DISABLED
#define MOTOR3_PWM_LINE_LIN2    PAL_LINE(GPIOB, 3)


static PWMDriver    *pwm1Driver = &PWMD1; 
static PWMDriver    *pwm2Driver = &PWMD2;
static PWMDriver    *pwm3Driver = &PWMD3;
static PWMDriver    *pwm4Driver = &PWMD4; 
 

/*============================================================================*/
/* PWM CONFIGURATION STRUCTURES                                               */
/*============================================================================*/

PWMConfig pwm1conf = {
    .frequency = PWM_FREQ,
    .period    = PWM1_PERIOD, 
    .callback  = NULL,
    .channels  = {
                  {.mode = PWM_OUTPUT_DISABLED,    .callback = NULL},
                  {.mode = MOTOR1_PE11_ACTIVE,     .callback = NULL},
                  {.mode = MOTOR1_PE13_ACTIVE,     .callback = NULL},
                  {.mode = MOTOR2_PE14_ACTIVE,     .callback = NULL}
                },
    .cr2        = 0,
    .dier       = 0
};


PWMConfig pwm2conf = {
    .frequency = PWM_FREQ,
    .period    = PWM1_PERIOD, 
    .callback  = NULL,
    .channels  = {
                  {.mode = MOTOR2_PA0_ACTIVE,     .callback = NULL},
                  {.mode = MOTOR3_PB3_ACTIVE,      .callback = NULL},
                  {.mode = MOTOR2_PB10_ACTIVE,      .callback = NULL},
                  {.mode = MOTOR2_PB11_ACTIVE,     .callback = NULL}
                },
    .cr2        = 0,
    .dier       = 0
};


PWMConfig pwm3conf = {
    .frequency = PWM_FREQ,
    .period    = PWM1_PERIOD, 
    .callback  = NULL,
    .channels  = {
                  {.mode = MOTOR3_PC6_ACTIVE,     .callback = NULL},
                  {.mode = MOTOR3_PC7_ACTIVE,      .callback = NULL},
                  {.mode = PWM_OUTPUT_DISABLED,      .callback = NULL},
                  {.mode = PWM_OUTPUT_DISABLED,     .callback = NULL}
                },
    .cr2        = 0,
    .dier       = 0
};


PWMConfig pwm4conf = {
    .frequency = PWM_FREQ,
    .period    = PWM1_PERIOD, 
    .callback  = NULL,
    .channels  = {
                  {.mode = MOTOR3_PB6_ACTIVE,     .callback = NULL},
                  {.mode = PWM_OUTPUT_DISABLED,     .callback = NULL},
                  {.mode = MOTOR1_PD14_ACTIVE,     .callback = NULL},
                  {.mode = MOTOR1_PD15_ACTIVE,      .callback = NULL}
                },
    .cr2        = 0,
    .dier       = 0
};



/*============================================================================*/
/* LOW-LEVEL FUCTIONS FOR DRIVE CONTROL                                       */
/*============================================================================*/

/**
 * @brief   Disable all used pwd channels 
 */
void lldDisableAllChannels( void )
{
    // Motor 1
    pwmDisableChannel( pwm4Driver, MOTOR1_PWM4CH_HIN1 ); 
    pwmDisableChannel( pwm4Driver, MOTOR1_PWM4CH_LIN1 ); 
    pwmDisableChannel( pwm1Driver, MOTOR1_PWM1CH_HIN2 ); 
    pwmDisableChannel( pwm1Driver, MOTOR1_PWM1CH_LIN2 ); 
    //Motor 2 
    pwmDisableChannel( pwm2Driver, MOTOR2_PWM2CH_HIN1 ); 
    pwmDisableChannel( pwm2Driver, MOTOR2_PWM2CH_LIN1 ); 
    pwmDisableChannel( pwm2Driver, MOTOR2_PWM2CH_HIN2 ); 
    pwmDisableChannel( pwm1Driver, MOTOR2_PWM1CH_LIN2 );
    //Motor 3
    pwmDisableChannel( pwm3Driver, MOTOR3_PWM3CH_HIN1 );
    pwmDisableChannel( pwm4Driver, MOTOR3_PWM4CH_LIN1 );
    pwmDisableChannel( pwm3Driver, MOTOR3_PWM3CH_HIN2 );
    pwmDisableChannel( pwm2Driver, MOTOR3_PWM2CH_LIN2 );
}

/**
 * @brief   Configure all lines to be used for PWM 
 */
void lldConfigureLineMode( void )
{
    // Motor 1
    palSetLineMode( MOTOR1_PWM_LINE_HIN1, PAL_MODE_ALTERNATE(2) );
    palSetLineMode( MOTOR1_PWM_LINE_LIN1, PAL_MODE_ALTERNATE(2) );
    palSetLineMode( MOTOR1_PWM_LINE_HIN2, PAL_MODE_ALTERNATE(1) );
    palSetLineMode( MOTOR1_PWM_LINE_LIN2, PAL_MODE_ALTERNATE(1) );
    // Motor 2
    palSetLineMode( MOTOR2_PWM_LINE_HIN1, PAL_MODE_ALTERNATE(1) ); 
    palSetLineMode( MOTOR2_PWM_LINE_LIN1, PAL_MODE_ALTERNATE(1) ); 
    palSetLineMode( MOTOR2_PWM_LINE_HIN2, PAL_MODE_ALTERNATE(1) ); 
    palSetLineMode( MOTOR2_PWM_LINE_LIN2, PAL_MODE_ALTERNATE(1) ); 
    // Motor 3
    palSetLineMode( MOTOR3_PWM_LINE_HIN1, PAL_MODE_ALTERNATE(2) );
    palSetLineMode( MOTOR3_PWM_LINE_LIN1, PAL_MODE_ALTERNATE(2) );
    palSetLineMode( MOTOR3_PWM_LINE_HIN2, PAL_MODE_ALTERNATE(2) );
    palSetLineMode( MOTOR3_PWM_LINE_LIN2, PAL_MODE_ALTERNATE(1) );
}


static bool isInitialized   = false; 

static float motor_b        = 0; 
static float motor_k        = 0; 

/**
 * @brief   Initialize periphery connected to driver control
 * @note    Stable for repeated calls
 */
void lldControlInit( void )
{
    if( isInitialized )
        return; 
    
    /*** PWM pins configuration ***/
    lldConfigureLineMode();

    motor_b = LLD_DUTY_MIN; 
    motor_k = ( PWM_PERIOD - motor_b ) / 100;

    pwmStart( pwm1Driver, &pwm1conf ); 
    pwmStart( pwm2Driver, &pwm2conf );
    pwmStart( pwm3Driver, &pwm3conf );
    pwmStart( pwm4Driver, &pwm4conf );
    
    // to avoid noise in contacts
    lldDisableAllChannels();

    isInitialized = true; 
}

/**
 * @brief   Set power (in ticks) for motor
 * @param
 *          motor_num       order number of motor [1; 3]
 *          duty            duty cycle [ticks]
 *          dir             direction of rotation [FORWARD, BACKWARD]
 */
void lldControlSetRawMotorPower( uint8_t motor_num, uint32_t duty, lldMotorDirection_t dir )
{
    duty = CLIP_VALUE( duty, 0, PWM1_PERIOD ); 
    motor_num = CLIP_VALUE( motor_num, 1, 3 ); 

    if( dir == FORWARD )
    {
        switch( motor_num )
        {
            case 1:
                pwmEnableChannel( pwm4Driver, MOTOR1_PWM4CH_HIN1, duty );
                // FIX T * (1 - D) - 10 mks !!!! 
                // pwmEnableChannel( pwm4Driver, MOTOR1_PWM4CH_LIN1, duty ); 
                pwmDisableChannel( pwm1Driver, MOTOR1_PWM1CH_HIN2 ); 
                pwmEnableChannel( pwm1Driver, MOTOR1_PWM1CH_LIN2, duty ); 
                break;

            case 2: 
                pwmDisableChannel( pwm1Driver, MOTOR2_PWM1CH_B );
                pwmEnableChannel( pwm1Driver, MOTOR2_PWM1CH_F, duty ); 
                break; 
            
            case 3: 
                pwmDisableChannel( pwm2Driver, MOTOR3_PWM2CH_B );
                pwmEnableChannel( pwm2Driver, MOTOR3_PWM2CH_F, duty ); 
                break; 

            default:
                break;
        }
    }
    else
    {
        switch( motor_num )
        {
            case 1:
                pwmDisableChannel( pwm4Driver, MOTOR1_PWM4CH_F ); 
                pwmEnableChannel( pwm1Driver, MOTOR1_PWM1CH_B, duty ); 
                break;

            case 2: 
                pwmDisableChannel( pwm1Driver, MOTOR2_PWM1CH_F );
                pwmEnableChannel( pwm1Driver, MOTOR2_PWM1CH_B, duty ); 
                break; 

            case 3: 
                pwmDisableChannel( pwm2Driver, MOTOR3_PWM2CH_F );
                pwmEnableChannel( pwm2Driver, MOTOR3_PWM2CH_B, duty ); 
                break; 

            default:
                break;
        }
    }
}

/**
 * @brief   Set power for motor
 * @param   inputPrc   Motor power value [-100 100]
 */
void lldControlSetMotorPower( uint8_t motor_num, lldControlValue_t inputPrc )
{
    inputPrc = CLIP_VALUE( inputPrc, LLD_MOTOR_MIN_PRC, LLD_MOTOR_MAX_PRC );
    motor_num = CLIP_VALUE( motor_num, 1, 3 ); 

    uint32_t duty = abs(inputPrc) * motor_k + motor_b; 

    if( inputPrc >= 0 )    // forward (clock-wise) rotation 
    {
        lldControlSetRawMotorPower( motor_num, duty, FORWARD );
    }
    else if( inputPrc <= 0 )                // backward (counterclock-wise) rotation
    {
        lldControlSetRawMotorPower( motor_num, duty, BACKWARD );
    }
}
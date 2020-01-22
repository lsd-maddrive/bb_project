#include <tests.h>
#include <lld_control.h> 

/*============================================================================*/
/* PWM CONFIGURATION PINS                                                     */
/*============================================================================*/

/**************** MOTOR 1 ****************/
#define MOTOR1_PWM4CH_F     3
#define MOTOR1_PD15_ACTIVE   PWM_OUTPUT_ACTIVE_HIGH
#define MOTOR1_PD15_DISABLE  PWM_OUTPUT_DISABLED
#define MOTOR1_PWM_LINE_F   PAL_LINE(GPIOD, 15)

#define MOTOR1_PWM1CH_B     1
#define MOTOR1_PE11_ACTIVE  PWM_OUTPUT_ACTIVE_HIGH
#define MOTOR1_PE11_DIASBLE PWM_OUTPUT_DISABLED
#define MOTOR1_PWM_LINE_B   PAL_LINE(GPIOE, 11)

/**************** MOTOR 2 ****************/
#define MOTOR2_PWM1CH_F     2
#define MOTOR2_PE13_ACTIVE  PWM_OUTPUT_ACTIVE_HIGH
#define MOTOR2_PE13_DISABLE PWM_OUTPUT_DISABLED
#define MOTOR2_PWM_LINE_F   PAL_LINE(GPIOE, 13)

#define MOTOR2_PWM1CH_B     3
#define MOTOR2_PE14_ACTIVE  PWM_OUTPUT_ACTIVE_HIGH
#define MOTOR2_PE14_DISABLE PWM_OUTPUT_DISABLED
#define MOTOR2_PWM_LINE_B   PAL_LINE(GPIOE, 14)

/**************** MOTOR 3 ****************/
#define MOTOR3_PWM2CH_F     2
#define MOTOR3_PB10_ACTIVE  PWM_OUTPUT_ACTIVE_HIGH
#define MOTOR3_PB10_DISABLE PWM_OUTPUT_DISABLED
#define MOTOR3_PWM_LINE_F   PAL_LINE(GPIOB, 10)

#define MOTOR3_PWM2CH_B     3
#define MOTOR3_PB11_ACTIVE  PWM_OUTPUT_ACTIVE_HIGH
#define MOTOR3_PB11_DISABLE PWM_OUTPUT_DISABLED
#define MOTOR3_PWM_LINE_B   PAL_LINE(GPIOB, 11)

static PWMDriver    *pwm1Driver = &PWMD1; 
static PWMDriver    *pwm4Driver = &PWMD4; 
static PWMDriver    *pwm2Driver = &PWMD2; 

/*============================================================================*/
/* PWM CONFIGURATION STRUCTURES                                               */
/*============================================================================*/
// Motor 1 (Backward) && Motor 2 
PWMConfig pwm1conf = {
    .frequency = PWM1_FREQ,
    .period    = PWM1_PERIOD, 
    .callback  = NULL,
    .channels  = {
                  {.mode = PWM_OUTPUT_DISABLED,    .callback = NULL},
                  {.mode = MOTOR1_PE11_ACTIVE,     .callback = NULL},
                  {.mode = MOTOR2_PE13_ACTIVE,     .callback = NULL},
                  {.mode = MOTOR2_PE14_ACTIVE,     .callback = NULL}
                },
    .cr2        = 0,
    .dier       = 0
};

// Motor 1 (Forward)
PWMConfig pwm4conf = {
    .frequency = PWM1_FREQ,
    .period    = PWM1_PERIOD, 
    .callback  = NULL,
    .channels  = {
                  {.mode = PWM_OUTPUT_DISABLED,     .callback = NULL},
                  {.mode = PWM_OUTPUT_DISABLED,     .callback = NULL},
                  {.mode = PWM_OUTPUT_DISABLED,     .callback = NULL},
                  {.mode = MOTOR1_PD15_ACTIVE,      .callback = NULL}
                },
    .cr2        = 0,
    .dier       = 0
};

// Motor 3 
PWMConfig pwm2conf = {
    .frequency = PWM1_FREQ,
    .period    = PWM1_PERIOD, 
    .callback  = NULL,
    .channels  = {
                  {.mode = PWM_OUTPUT_DISABLED,     .callback = NULL},
                  {.mode = PWM_OUTPUT_DISABLED,      .callback = NULL},
                  {.mode = MOTOR3_PB10_ACTIVE,      .callback = NULL},
                  {.mode = MOTOR3_PB11_ACTIVE,     .callback = NULL}
                },
    .cr2        = 0,
    .dier       = 0
};

/*============================================================================*/
/* LOW-LEVEL FUCTIONS FOR DRIVE CONTROL                                       */
/*============================================================================*/

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
    palSetLineMode( MOTOR1_PWM_LINE_F, PAL_MODE_ALTERNATE(2) );
    palSetLineMode( MOTOR1_PWM_LINE_B, PAL_MODE_ALTERNATE(1) );

    palSetLineMode( MOTOR2_PWM_LINE_F, PAL_MODE_ALTERNATE(1) ); 
    palSetLineMode( MOTOR2_PWM_LINE_B, PAL_MODE_ALTERNATE(1) ); 

    palSetLineMode( MOTOR3_PWM_LINE_F, PAL_MODE_ALTERNATE(1) );
    palSetLineMode( MOTOR3_PWM_LINE_B, PAL_MODE_ALTERNATE(1) );
    /*****************************/

    motor_b = LLD_DUTY_MIN; 
    motor_k = ( PWM1_PERIOD - motor_b ) / 100;

    pwmStart( pwm1Driver, &pwm1conf ); 
    pwmStart( pwm4Driver, &pwm4conf );
    pwmStart( pwm2Driver, &pwm2conf );

    // to avoid noise in contacts
    pwmDisableChannel( pwm4Driver, MOTOR1_PWM4CH_F ); 
    pwmDisableChannel( pwm1Driver, MOTOR1_PWM1CH_B ); 

    pwmDisableChannel( pwm1Driver, MOTOR2_PWM1CH_F ); 
    pwmDisableChannel( pwm1Driver, MOTOR2_PWM1CH_B ); 

    pwmDisableChannel( pwm2Driver, MOTOR3_PWM2CH_F ); 
    pwmDisableChannel( pwm2Driver, MOTOR3_PWM2CH_B );
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
                pwmDisableChannel( pwm1Driver, MOTOR1_PWM1CH_B ); 
                pwmEnableChannel( pwm4Driver, MOTOR1_PWM4CH_F, duty ); 
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
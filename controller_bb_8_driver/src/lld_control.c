#include <tests.h>
#include <lld_control.h> 

/*============================================================================*/
/* PWM CONFIGURATION PINS                                                     */
/*============================================================================*/

/**************** MOTOR 1 (PWM 1) ********************/
#define MOTOR1_PWM1CH_HIN1      1
#define MOTOR1_PE11_ACTIVE      PWM_OUTPUT_ACTIVE_HIGH
#define MOTOR1_PE11_DISABLE     PWM_OUTPUT_DISABLED
#define MOTOR1_PWM_LINE_HIN1    PAL_LINE(GPIOE, 11)

#define MOTOR1_PWM1CH_LIN1      1  // N
#define MOTOR1_PWM_LINE_LIN1    PAL_LINE(GPIOE, 10)

#define MOTOR1_PWM1CH_HIN2      2
#define MOTOR1_PE13_ACTIVE      PWM_OUTPUT_ACTIVE_HIGH
#define MOTOR1_PE13_DISABLE     PWM_OUTPUT_DISABLED
#define MOTOR1_PWM_LINE_HIN2    PAL_LINE(GPIOE, 13)

#define MOTOR1_PWM1CH_LIN2      2  // N
#define MOTOR1_PWM_LINE_LIN2    PAL_LINE(GPIOE, 12)

/**************** MOTOR 2 (PWM 8) ********************/
#define MOTOR2_PWM8CH_HIN1      0
#define MOTOR2_PC6_ACTIVE       PWM_OUTPUT_ACTIVE_HIGH
#define MOTOR2_PC6_DISABLE      PWM_OUTPUT_DISABLED
#define MOTOR2_PWM_LINE_HIN1    PAL_LINE(GPIOC, 6)

#define MOTOR2_PWM8CH_LIN1      0  // N
#define MOTOR2_PWM_LINE_LIN1    PAL_LINE(GPIOA, 5)

#define MOTOR2_PWM8CH_HIN2      1
#define MOTOR2_PC7_ACTIVE       PWM_OUTPUT_ACTIVE_HIGH
#define MOTOR2_PC7_DISABLE      PWM_OUTPUT_DISABLED
#define MOTOR2_PWM_LINE_HIN2    PAL_LINE(GPIOC, 7)

#define MOTOR2_PWM8CH_LIN2      1  // N
#define MOTOR2_PWM_LINE_LIN2    PAL_LINE(GPIOB, 0)

/**************** MOTOR 3 (PWM1 + PWM8) ********************/
#define MOTOR3_PWM8CH_HIN1      2
#define MOTOR3_PC8_ACTIVE       PWM_OUTPUT_ACTIVE_HIGH
#define MOTOR3_PC8_DISABLE      PWM_OUTPUT_DISABLED
#define MOTOR3_PWM_LINE_HIN1    PAL_LINE(GPIOC, 8)

#define MOTOR3_PWM8CH_LIN1      2  // N
#define MOTOR3_PWM_LINE_LIN1    PAL_LINE(GPIOB, 1)

#define MOTOR3_PWM1CH_HIN2      0
#define MOTOR3_PE9_ACTIVE       PWM_OUTPUT_ACTIVE_HIGH
#define MOTOR3_PE9_DISABLE      PWM_OUTPUT_DISABLED
#define MOTOR3_PWM_LINE_HIN2    PAL_LINE(GPIOE, 9)

#define MOTOR3_PWM1CH_LIN2      0  // N
#define MOTOR3_PWM_LINE_LIN2    PAL_LINE(GPIOE, 8)


static PWMDriver    *pwm1Driver = &PWMD1;
static PWMDriver    *pwm8Driver = &PWMD8;

/*============================================================================*/
/* PWM CONFIGURATION STRUCTURES                                               */
/*============================================================================*/

PWMConfig pwm1conf = {
    .frequency = PWM_FREQ,
    .period    = PWM_PERIOD,
    .callback  = NULL,
    .channels  = {
                  {.mode = MOTOR3_PE9_ACTIVE  | PWM_COMPLEMENTARY_OUTPUT_ACTIVE_HIGH,    .callback = NULL},
                  {.mode = MOTOR1_PE11_ACTIVE | PWM_COMPLEMENTARY_OUTPUT_ACTIVE_HIGH,    .callback = NULL},
                  {.mode = MOTOR1_PE13_ACTIVE | PWM_COMPLEMENTARY_OUTPUT_ACTIVE_HIGH,    .callback = NULL},
                  {.mode = PWM_OUTPUT_DISABLED,     .callback = NULL}
                },
    .cr2        = 0,
    .bdtr       = 500,    // break && dead-time HOW TO CONTROL YOU?????
    .dier       = 0
};


PWMConfig pwm8conf = {
    .frequency = PWM_FREQ,
    .period    = PWM_PERIOD,
    .callback  = NULL,
    .channels  = {
                  {.mode = MOTOR2_PC6_ACTIVE | PWM_COMPLEMENTARY_OUTPUT_ACTIVE_HIGH,    .callback = NULL},
                  {.mode = MOTOR2_PC7_ACTIVE | PWM_COMPLEMENTARY_OUTPUT_ACTIVE_HIGH,    .callback = NULL},
                  {.mode = MOTOR3_PC8_ACTIVE | PWM_COMPLEMENTARY_OUTPUT_ACTIVE_HIGH,    .callback = NULL},
                  {.mode = PWM_OUTPUT_DISABLED,     .callback = NULL}
                },
    .cr2        = 0,
    .bdtr       = 500,    // break && dead-time HOW TO CONTROL YOU?????
    .dier       = 0
};

/*============================================================================*/
/* LOW-LEVEL FUCTIONS FOR DRIVE CONTROL                                       */
/*============================================================================*/

/**
 * @brief   Disable all used PWM channels
 */
void lldDisableAllChannels( void )
{
    // Motor 1
    pwmDisableChannel( pwm1Driver, MOTOR1_PWM1CH_HIN1 );
    pwmDisableChannel( pwm1Driver, MOTOR1_PWM1CH_LIN1 ); // not working
    pwmDisableChannel( pwm1Driver, MOTOR1_PWM1CH_HIN2 ); 
    pwmDisableChannel( pwm1Driver, MOTOR1_PWM1CH_LIN2 ); // not working

    //Motor 2 
    pwmDisableChannel( pwm8Driver, MOTOR2_PWM8CH_HIN1 );
    pwmDisableChannel( pwm8Driver, MOTOR2_PWM8CH_LIN1 );
    pwmDisableChannel( pwm8Driver, MOTOR2_PWM8CH_HIN2 );
    pwmDisableChannel( pwm8Driver, MOTOR2_PWM8CH_LIN2 );

    //Motor 3
    pwmDisableChannel( pwm8Driver, MOTOR3_PWM8CH_HIN1 );
    pwmDisableChannel( pwm8Driver, MOTOR3_PWM8CH_LIN1 );
    pwmDisableChannel( pwm1Driver, MOTOR3_PWM1CH_HIN2 );
    pwmDisableChannel( pwm1Driver, MOTOR3_PWM1CH_LIN2 );
}

/**
 * @brief   Configure all lines to be used for PWM 
 */
void lldConfigureLineMode( void )
{
    // Motor 1
    palSetLineMode( MOTOR1_PWM_LINE_HIN1, PAL_MODE_ALTERNATE(1) );
    palSetLineMode( MOTOR1_PWM_LINE_LIN1, PAL_MODE_ALTERNATE(1) );
    palSetLineMode( MOTOR1_PWM_LINE_HIN2, PAL_MODE_ALTERNATE(1) );
    palSetLineMode( MOTOR1_PWM_LINE_LIN2, PAL_MODE_ALTERNATE(1) );

    // Motor 2
    palSetLineMode( MOTOR2_PWM_LINE_HIN1, PAL_MODE_ALTERNATE(3) );
    palSetLineMode( MOTOR2_PWM_LINE_LIN1, PAL_MODE_ALTERNATE(3) );
    palSetLineMode( MOTOR2_PWM_LINE_HIN2, PAL_MODE_ALTERNATE(3) );
    palSetLineMode( MOTOR2_PWM_LINE_LIN2, PAL_MODE_ALTERNATE(3) );

    // Motor 3
    palSetLineMode( MOTOR3_PWM_LINE_HIN1, PAL_MODE_ALTERNATE(3) );
    palSetLineMode( MOTOR3_PWM_LINE_LIN1, PAL_MODE_ALTERNATE(3) );
    palSetLineMode( MOTOR3_PWM_LINE_HIN2, PAL_MODE_ALTERNATE(1) );
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
    
    motor_b = LLD_DUTY_MIN; 
    motor_k = ( PWM_PERIOD - motor_b ) / 100;

    /*** PWM pins configuration ***/
    lldConfigureLineMode();

    pwmStart( pwm1Driver, &pwm1conf );
    pwmStart( pwm8Driver, &pwm8conf );
    
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
    duty = CLIP_VALUE( duty, 0, PWM_PERIOD );
    motor_num = CLIP_VALUE( motor_num, 1, 3 ); 

    if( dir == FORWARD )
    {
        switch( motor_num )
        {
            case 1:
                pwmEnableChannel ( pwm1Driver, MOTOR1_PWM1CH_HIN1, duty );
                pwmDisableChannel( pwm1Driver, MOTOR1_PWM1CH_HIN2 );
                break;

            case 2:
                pwmEnableChannel ( pwm8Driver, MOTOR2_PWM8CH_HIN1, duty );
                pwmDisableChannel( pwm8Driver, MOTOR2_PWM8CH_HIN2 );
                break; 

            case 3: 
                pwmEnableChannel ( pwm8Driver, MOTOR3_PWM8CH_HIN1, duty );
                pwmDisableChannel( pwm1Driver, MOTOR3_PWM1CH_HIN2 );
                break; 

            default:
                break;
        }
    }
    else if( dir == BACKWARD )
    {
        switch( motor_num )
        {
            case 1:
                pwmEnableChannel ( pwm1Driver, MOTOR1_PWM1CH_HIN2, duty );
                pwmDisableChannel( pwm1Driver, MOTOR1_PWM1CH_HIN1 );
                break;

            case 2:
                pwmEnableChannel ( pwm8Driver, MOTOR2_PWM8CH_HIN2, duty );
                pwmDisableChannel( pwm8Driver, MOTOR2_PWM8CH_HIN1 );
                break; 

            case 3: 
                pwmEnableChannel ( pwm1Driver, MOTOR3_PWM1CH_HIN2, duty );
                pwmDisableChannel( pwm8Driver, MOTOR3_PWM8CH_HIN1 );
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

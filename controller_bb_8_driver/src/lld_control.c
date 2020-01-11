#include <tests.h>
#include <lld_control.h> 

#define PWM1_FREQ    1000000
// PWM Period 50 Hz 
#define PWM1_PERIOD  20000

/*============================================================================*/
/* PWM CONFIGURATION PINS                                                     */
/*============================================================================*/
#define MOTOR1_PWM1CH_F     0
#define MOTOR1_PE9_ACTIVE   PWM_OUTPUT_ACTIVE_HIGH
#define MOTOR1_PE9_DISABLE  PWM_OUTPUT_DISABLED
#define MOTOR1_PWM_LINE_F   PAL_LINE(GPIOE, 9)

#define MOTOR1_PWM1CH_B     1
#define MOTOR1_PE11_ACTIVE  PWM_OUTPUT_ACTIVE_HIGH
#define MOTOR1_PE11_DIASBLE PWM_OUTPUT_DISABLED
#define MOTOR1_PWM_LINE_B   PAL_LINE(GPIOE, 11)

#define MOTOR2_PWM1CH_F     2
#define MOTOR2_PE13_ACTIVE  PWM_OUTPUT_ACTIVE_HIGH
#define MOTOR2_PE13_DISABLE PWM_OUTPUT_DISABLED
#define MOTOR2_PWM_LINE_F   PAL_LINE(GPIOE, 13)

#define MOTOR2_PWM1CH_B     3
#define MOTOR2_PE14_ACTIVE  PWM_OUTPUT_ACTIVE_HIGH
#define MOTOR2_PE14_DISABLE PWM_OUTPUT_DISABLED
#define MOTOR2_PWM_LINE_B   PAL_LINE(GPIOE, 14)

static PWMDriver    *pwmDriver = &PWMD1; 

/*============================================================================*/
/* PWM CONFIGURATION STRUCTURES                                               */
/*============================================================================*/
PWMConfig pwm1conf = {
    .frequency = PWM1_FREQ,
    .period    = PWM1_PERIOD, 
    .callback  = NULL,
    .channels  = {
                  {.mode = MOTOR1_PE9_ACTIVE,      .callback = NULL},
                  {.mode = MOTOR1_PE11_ACTIVE,     .callback = NULL},
                  {.mode = MOTOR2_PE13_ACTIVE,    .callback = NULL},
                  {.mode = MOTOR2_PE14_ACTIVE,    .callback = NULL}
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
    palSetLineMode( MOTOR1_PWM_LINE_F, PAL_MODE_ALTERNATE(1) );
    palSetLineMode( MOTOR1_PWM_LINE_B, PAL_MODE_ALTERNATE(1) );
    palSetLineMode( MOTOR2_PWM_LINE_F, PAL_MODE_ALTERNATE(1) ); 
    palSetLineMode( MOTOR2_PWM_LINE_B, PAL_MODE_ALTERNATE(1) ); 

    motor_b = LLD_DUTY_MIN; 
    motor_k = (LLD_DUTY_MAX - motor_b) / 100;

    pwmStart( pwmDriver, &pwm1conf ); 
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
                pwmDisableChannel( pwmDriver, MOTOR1_PWM1CH_B); 
                pwmEnableChannel( pwmDriver, MOTOR1_PWM1CH_F, duty); 
                break;

            case 2: 
                pwmDisableChannel( pwmDriver, MOTOR2_PWM1CH_B);
                pwmEnableChannel( pwmDriver, MOTOR2_PWM1CH_F, duty); 
                break; 

            // TO DO:
            //      Add case 3 for motor 3 

            default:
                // Error
                break;
        }
    }
    else
    {
        switch( motor_num )
        {
            case 1:
                pwmDisableChannel( pwmDriver, MOTOR1_PWM1CH_F); 
                pwmEnableChannel( pwmDriver, MOTOR1_PWM1CH_B, duty); 
                break;

            case 2: 
                pwmDisableChannel( pwmDriver, MOTOR2_PWM1CH_F);
                pwmEnableChannel( pwmDriver, MOTOR2_PWM1CH_B, duty); 
                break; 

            // TO DO:
            //      Add case 3 for motor 3 

            default:
                // Error
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

    uint32_t duty = inputPrc * motor_k + motor_b; 

    if( inputPrc >= 0 )    // forward (clock-wise) rotation 
    {
        lldControlSetRawMotorPower( motor_num, duty, FORWARD );
    }
    else                // backward (counterclock-wise) rotation
    {
        lldControlSetRawMotorPower( motor_num, duty, BACKWARD );
    }
}
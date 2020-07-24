#ifndef INCLUDE_LLD_CONTROL_H_
#define INCLUDE_LLD_CONTROL_H_

#define PWM_FREQ        1000000
#define PWM_PERIOD      10000   // 100 Hz
#define DEAD_TIME_MKS   5       // time in mks
#define VT_PERIOD_MKS   1       // 1 mks
#define LLD_DUTY_MIN    0

typedef int32_t lldControlValue_t;

typedef enum motor_dir {
    FORWARD     = 1, 
    BACKWARD    = 0,
    UNKNOWN     = 100
} lldMotorDirection_t; 


/**
 * @brief   Get specified by user direction of motor rotation
 */
lldMotorDirection_t lldGetMotorDirection( void );

/**
 * @brief   Get duty cycle for manual PWM in us
 */
float lldGetReverseDutyUS( void );

/**
 * @brief   Disable all used pwd channels
 */
void lldDisableAllChannels( void );

/**
 * @brief   Configure all lines to be used for PWM
 */
void lldConfigureLineMode( void );

/**
 * @brief   Initialize periphery connected to driver control
 * @note    Stable for repeated calls
 */
void lldControlInit( void ); 

/**
 * @brief   Set power (in ticks) for motor
 * @param
 *          motor_num       order number of motor [1; 3]
 *          duty            duty cycle [ticks]
 *          dir             direction of rotation [FORWARD, BACKWARD]
 */
void lldControlSetRawMotorPower( uint8_t motor_num, uint32_t duty, lldMotorDirection_t dir );

/**
 * @brief   Set power for motor
 * @param   inputPrc   Motor power value [-100 100]
 */
void lldControlSetMotorPower( uint8_t motor_num, lldControlValue_t inputPrc );

#endif 

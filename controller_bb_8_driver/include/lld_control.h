#ifndef INCLUDE_LLD_CONTROL_H_
#define INCLUDE_LLD_CONTROL_H_

#define PWM1_FREQ       1000000
#define PWM1_PERIOD     10000
#define LLD_DUTY_MIN    0

typedef int32_t lldControlValue_t;

typedef enum motor_dir {
    FORWARD     = 1, 
    BACKWARD    = 0
} lldMotorDirection_t; 

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
#ifndef INCLUDE_COMMON_H_
#define INCLUDE_COMMON_H_

#include <ch.h>
#include <hal.h>

#include <chprintf.h>

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include <i2c.h>

#ifdef __cplusplus
extern "C" {
#endif

#define     PROGRAM_ROUTINE_MASTER                      0
//  LLD MOTOR TESTS
#define     PROGRAM_MOTOR_DIRECTION_TEST                1
#define     PROGRAM_MOTOR_RAW_CONTROL_TEST              2
#define     PROGRAM_MOTOR_PRC_CONTROL_TEST              3
#define     PROGRAM_MOTOR_MATLAB_FREQ_TEST              4
//  LLD ENCODER TESTS
#define     PROGRAM_ENCODER_TEST                        5
//  WHEEL ODOMETRY TESTS
#define     PROGRAM_MOTOR_SPEED_TEST                    6
#define     PROGRAM_WHEEL_SPEED_TEST                    7
//  WHEEL CONTROL SYSTEM TESTS
#define     PROGRAM_WHEEL_CONTROL_MATLAB                8
#define     PROGRAM_ONE_WHEEL_CONTROLLER_MATLAB         9
//  ROBOT ODOMETRY TESTS
#define     PROGRAM_LINEAR_SPEED_MATLAB_TEST            10
#define     PROGRAM_ROBOT_ODOMETRY_TEST                 11
//  GYROSCOPE TEST
#define		  PROGRAM_GYROSCOPE_TEST						          12
//  JOYSTICK TESTS
#define     PROGRAM_JOYSTICK_TEST                       13
#define     PROGRAM_ROBOT_WITH_JOYSTICK_TEST            14
//  LOGGER TEST
#define     PROGRAM_LOGGER_TEST                         15

#define     MAIN_PROGRAM_ROUTINE                        PROGRAM_GYROSCOPE_TEST


/*============================================================================*/
/* MACROS                                                                     */
/*============================================================================*/

#define CLIP_VALUE(x, min, max) (((int32_t)x) < (min) ? (min) :      \
                                 ((int32_t)x) > ((int32_t)max) ? (max) : (x))

/*============================================================================*/
/* LLD LIMITS                                                                 */
/*============================================================================*/

#define LLD_MOTOR_MAX_PRC       80
#define LLD_MOTOR_MIN_PRC       (-80)

/*============================================================================*/
/* SPEED LIMITS                                                                 */
/*============================================================================*/

#define WHEEL_SPEED_MAX_RPS     1.5
#define WHEEL_SPEED_MIN_RPS     (-1.5)

/*============================================================================*/
/* ROBOT CONFIGURATION CONTANTS                                                                 */
/*============================================================================*/

#define MOTOR_GAIN               27
#define WHEEL_RADIUS_M           (float)0.05
#define BASE_WHEEL_DIST_M        (float)0.2


/*============================================================================*/
/* DEBUG                                                                      */
/*============================================================================*/

void debug_stream_init( void );
void dbgprintf( const char* format, ... );


/*============================================================================*/
/* Common units                                                            */
/*============================================================================*/

typedef enum {
  A,
  B,
  C
} motorNumberValue_t;

// PID Controller
typedef float   controllerRate_t;

typedef struct {
  controllerRate_t kp;
  controllerRate_t ki;
  controllerRate_t kd;

  controllerRate_t intgSaturation;
  controllerRate_t propDeadZone;
  int16_t controlDeadZone;

} pidControllerValue_t;


/**
 * @brief   Initialize EXT driver with empty config
 * @note    Safe to call any times, it checks state of previous call
 * @note    Must be called before EXT driver work
 */
void commonExtDriverInit ( void );


/**
 * @brief   Initialization of base units for main programm
 */
void mainUnitsInit( void );

/**
 * @brief   Main control program
 */
void mainControlTask( void );


#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_COMMON_H_ */

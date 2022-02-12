#ifndef INCLUDE_LLD_ENCODER_H_
#define INCLUDE_LLD_ENCODER_H_

#include "common.h"

typedef int32_t     encoderTicksValue_t;
typedef float       encoderRevsValue_t;


/**
 * @brief   Initialize periphery connected to encoder
 * @note    Stable for repeated calls
 */
void lldEncoderInit( void );

/**
 * @brief   Get number of encoder ticks
 * @note    Max number of ticks is defined by MAX_TICK_NUM
 * @return  Encoder ticks number depends on direction of rotation
 *          [0; ENC_MAX_TICK_NUM]
 *          after ENC_MAX_TICK_NUM it resets
 */
encoderTicksValue_t lldGetEncoderTicks( motorNumberValue_t number );

/**
 * @brief   Get direction of encoder rotation
 * @return  clockwise           -> 0
 *          counterclockwise    -> 1
 */
bool lldGetEncoderDirection( motorNumberValue_t number );

/**
 * @brief   Get number of encoder revolutions [float]
 * @note    1 revolution = MAX_TICK_NUM ticks
 * @return  Encoder revolutions number depends on direction of rotation
 */
encoderRevsValue_t lldGetEncoderRevs( motorNumberValue_t number );

/*
 * @brief   Reset all variable for lld-encoder unit
 */
void lldEncoderReset( motorNumberValue_t number );

#endif

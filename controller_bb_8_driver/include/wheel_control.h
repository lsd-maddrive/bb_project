#ifndef INCLUDE_WHEEL_CONTROL_H_
#define INCLUDE_WHEEL_CONTROL_H_

#include "lld_control.h"
#include "odometry.h"

#define VT_WHEEL_CONTROL_MS      10

typedef float   wheelSpeedValue_t;



/**
 * @brief       Initialization of unit with control system for each wheel
 */
void wheelControlInit( void );

/**
 * @brief       Permeation is enabled,
 *              control system is enabled
 */
void wheelControlSetPermeation( void );

/**
 * @brief       Permeation is disabled,
 *              control system is disabled
 */
void wheelControlResetPermeation( void );

/**
 * @brief       Set reference value of for specified wheel
 */
void wheelControlSetSpeed( wheelSpeedValue_t speed_val, motorNumberValue_t number, odometrySpeedUnit_t unit );

// TODO: CHECK wheelControlValuesPrc[number] !
/**
 * @brief       Get calculated control value of speed in percent
 *              for specified wheel in specified units
 */
lldControlValue_t wheelControlGetControlSpeed( motorNumberValue_t number, odometrySpeedUnit_t unit );

/*
 * @brief       Reset all components for PID-controller
 */
void wheelControlResetController( motorNumberValue_t number );

#endif /* INCLUDE_WHEEL_CONTROL_H_ */

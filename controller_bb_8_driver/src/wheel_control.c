#include "wheel_control.h"

static virtual_timer_t  wheel_control_vt;

bool permeation_flag = false;

// Array for control values in percent for each of wheels
lldControlValue_t       wheelControlValuesPrc[3] = {0, 0, 0};
// Array for reference value of wheel speed [RPS]
wheelSpeedValue_t       wheelSpeedRefValuesPRS[3] = {0, 0, 0};

/**************** WHEEL A ********************/
wheelSpeedValue_t       wheelSpeedRefRPS_A    = 0;
// I-part with saturation
float                   wheelSpeedPrevError_A = 0;
float                   wheelSpeedIntg_A      = 0;

pidControllerValue_t    wheelController_A = {
    .kp = 50,
    .ki = 3,
    .kd = 0,
    .intgSaturation = LLD_MOTOR_MAX_PRC,
    .propDeadZone = 0,
    .controlDeadZone = 5
};

/**************** WHEEL B ********************/
wheelSpeedValue_t       wheelSpeedRefRPS_B    = 0;
float                   wheelSpeedPrevError_B = 0;
float                   wheelSpeedIntg_B      = 0;

pidControllerValue_t    wheelController_B = {
    .kp = 50,
    .ki = 3,
    .kd = 0,
    .intgSaturation = LLD_MOTOR_MAX_PRC,
    .propDeadZone = 0,
    .controlDeadZone = 5
};

/**************** WHEEL C ********************/
wheelSpeedValue_t       wheelSpeedRefRPS_C    = 0;
float                   wheelSpeedPrevError_C = 0;
float                   wheelSpeedIntg_C      = 0;

pidControllerValue_t    wheelController_C = {
    .kp = 50,
    .ki = 3,
    .kd = 0,
    .intgSaturation = LLD_MOTOR_MAX_PRC,
    .propDeadZone = 0,
    .controlDeadZone = 5
};


static void wheel_control_vt_cb( void *arg )
{
    arg = arg;  // to avoid warnings

/*============================================================================*/
/* Calculation Area                                                            */
/*============================================================================*/

    if( permeation_flag )
    {
/**************** CS WHEEL A ********************/
      odometrySpeedValue_t wheelSpeedRPS_A_LPF = odometryGetWheelSpeed( A, REVS_PER_SEC );
// P-Part
      float wheelSpeedError_A = wheelSpeedRefValuesPRS[0] - wheelSpeedRPS_A_LPF;
// I-Part
      wheelSpeedIntg_A += wheelController_A.ki * wheelSpeedError_A;
// I-part with saturation
      wheelSpeedIntg_A = CLIP_VALUE(
            wheelSpeedIntg_A,
            -wheelController_A.intgSaturation,
            wheelController_A.intgSaturation
      );
// D-Part [not used]
      float wheelSpeedDif_A = wheelSpeedError_A - wheelSpeedPrevError_A;

      wheelSpeedPrevError_A = wheelSpeedError_A;

      wheelControlValuesPrc[0] = wheelController_A.kp * wheelSpeedError_A +
                                 wheelSpeedIntg_A +
                                 wheelController_A.kd * wheelSpeedDif_A;

      if( abs(wheelControlValuesPrc[0]) <= wheelController_A.controlDeadZone )
      {
          wheelControlValuesPrc[0] = 0;
      }

/**************** CS WHEEL B ********************/
      odometrySpeedValue_t wheelSpeedRPS_B_LPF = odometryGetWheelSpeed( B, REVS_PER_SEC );
// P-Part
      float wheelSpeedError_B = wheelSpeedRefValuesPRS[1] - wheelSpeedRPS_B_LPF;
// I-Part
      wheelSpeedIntg_B += wheelController_B.ki * wheelSpeedError_B;
      wheelSpeedIntg_B = CLIP_VALUE(
            wheelSpeedIntg_B,
            -wheelController_B.intgSaturation,
            wheelController_B.intgSaturation
      );
// D-Part [not used]
      float wheelSpeedDif_B = wheelSpeedError_B - wheelSpeedPrevError_B;

      wheelSpeedPrevError_B = wheelSpeedError_B;

      wheelControlValuesPrc[1] = wheelController_B.kp * wheelSpeedError_B +
                                 wheelSpeedIntg_B +
                                 wheelController_B.kd * wheelSpeedDif_B;
      if( abs(wheelControlValuesPrc[1]) <= wheelController_B.controlDeadZone )
      {
          wheelControlValuesPrc[1] = 0;
      }

/**************** CS WHEEL C ********************/
      odometrySpeedValue_t wheelSpeedRPS_C_LPF = odometryGetWheelSpeed( C, REVS_PER_SEC );
// P-Part
      float wheelSpeedError_C = wheelSpeedRefValuesPRS[2] - wheelSpeedRPS_C_LPF;
// I-Part
      wheelSpeedIntg_C += wheelController_C.ki * wheelSpeedError_C;
      wheelSpeedIntg_C = CLIP_VALUE(
            wheelSpeedIntg_C,
            -wheelController_C.intgSaturation,
            wheelController_C.intgSaturation
      );
// D-Part [not used]
      float wheelSpeedDif_C = wheelSpeedError_C - wheelSpeedPrevError_C;

      wheelSpeedPrevError_C = wheelSpeedError_C;

      wheelControlValuesPrc[2] = wheelController_C.kp * wheelSpeedError_C +
                                 wheelSpeedIntg_C +
                                 wheelController_C.kd * wheelSpeedDif_C;

      if( abs(wheelControlValuesPrc[2]) <= wheelController_C.controlDeadZone )
      {
          wheelControlValuesPrc[2] = 0;
      }
/*============================================================================*/
/* Set control in percent                                                             */
/*============================================================================*/
      lldControlSetMotorPower( A, wheelControlValuesPrc[0] );
      lldControlSetMotorPower( B, wheelControlValuesPrc[1] );
      lldControlSetMotorPower( C, wheelControlValuesPrc[2] );
    }

    chSysLockFromISR();
    chVTSetI(&wheel_control_vt, MS2ST( VT_WHEEL_CONTROL_MS ), wheel_control_vt_cb, NULL);
    chSysUnlockFromISR();
}

/**
 * @brief       Permeation is enabled,
 *              control system is enabled
 */
void wheelControlSetPermeation( void )
{
    permeation_flag = true;
}

/**
 * @brief       Permeation is disabled,
 *              control system is disabled
 */
void wheelControlResetPermeation( void )
{
    permeation_flag = false;
}

static bool isInitialized = false;

/**
 * @brief       Initialization of unit with control system for each wheel
 */
void wheelControlInit( void )
{
    if( isInitialized )
      return;

    lldControlInit( );
    odometryInit( );
    permeation_flag = false;

    chVTObjectInit(&wheel_control_vt);
    chVTSet( &wheel_control_vt, MS2ST( VT_WHEEL_CONTROL_MS ), wheel_control_vt_cb, NULL );

    isInitialized = true;
}

/**
 * @brief       Set reference value of for specified wheel
 */
void wheelControlSetSpeed( wheelSpeedValue_t speed_val, motorNumberValue_t number, odometrySpeedUnit_t unit )
{
    speed_val = CLIP_VALUE( speed_val, WHEEL_SPEED_MIN_RPS, WHEEL_SPEED_MAX_RPS);

    switch( unit )
    {
        case REVS_PER_SEC:
            wheelSpeedRefValuesPRS[number] = speed_val;
            break;

        default:
            wheelSpeedRefValuesPRS[number] = 0; // temporary maybe
            break;
    }
}

/**
 * @brief       Set reference value of for all wheel
 */
void wheelControlSetSpeedAllWheels( wheelSpeedValue_t speed_val, odometrySpeedUnit_t unit )
{
    speed_val = CLIP_VALUE( speed_val, WHEEL_SPEED_MIN_RPS, WHEEL_SPEED_MAX_RPS);

    switch( unit )
    {
        case REVS_PER_SEC:
            wheelSpeedRefValuesPRS[0] = speed_val; // A wheel
            wheelSpeedRefValuesPRS[1] = speed_val; // B wheel
            wheelSpeedRefValuesPRS[2] = speed_val; // C wheel
            break;

        default:
            wheelSpeedRefValuesPRS[0] = 0; // temporary maybe
            wheelSpeedRefValuesPRS[1] = 0; // temporary maybe
            wheelSpeedRefValuesPRS[2] = 0; // temporary maybe
            break;
    }

}

/**
 * @brief       Get calculated control value of speed in percent
 *              for specified wheel in specified units
 */
lldControlValue_t wheelControlGetControlSpeed( motorNumberValue_t number, odometrySpeedUnit_t unit )
{
    switch( unit )
    {
        case REVS_PER_SEC:
            return wheelControlValuesPrc[number];

        default:
            return 0;
            break;
    }
}

/*
 * @brief       Stop all wheels
 *              - PID config is not reseted
 *              - permeation flag is not reseted
 */
void wheelControlStopWheels( void )
{
    wheelSpeedRefValuesPRS[0] = 0;
    wheelSpeedRefValuesPRS[1] = 0;
    wheelSpeedRefValuesPRS[2] = 0;
}



/*
 * @brief       Reset all components for PID-controller
 *              for specified wheel
 */
void wheelControlResetController( motorNumberValue_t number )
{
    wheelSpeedRefValuesPRS[number] = 0;
    wheelControlValuesPrc[number] = 0;
    permeation_flag = false;

    lldControlSetMotorPower( number, wheelControlValuesPrc[number] );

    switch( number )
    {
        case A:
          wheelSpeedPrevError_A = 0;
          wheelSpeedIntg_A      = 0;
          break;

        case B:
          wheelSpeedPrevError_B = 0;
          wheelSpeedIntg_B      = 0;
          break;

        case C:
          wheelSpeedPrevError_C = 0;
          wheelSpeedIntg_C      = 0;
          break;

        default:
          break;
    }
}

/*
 * @brief       Reset all components for PID-controller
 *              for all wheels
 */
void wheelControlResetControllerAllWheels( void )
{
    wheelSpeedRefValuesPRS[0] = 0;
    wheelSpeedRefValuesPRS[1] = 0;
    wheelSpeedRefValuesPRS[2] = 0;

    wheelControlValuesPrc[0] = 0;
    wheelControlValuesPrc[1] = 0;
    wheelControlValuesPrc[2] = 0;
    permeation_flag = false;

    lldControlSetMotorPower( A, wheelControlValuesPrc[0] );
    lldControlSetMotorPower( B, wheelControlValuesPrc[1] );
    lldControlSetMotorPower( C, wheelControlValuesPrc[2] );

    wheelSpeedPrevError_A = 0;
    wheelSpeedIntg_A      = 0;

    wheelSpeedPrevError_B = 0;
    wheelSpeedIntg_B      = 0;

    wheelSpeedPrevError_C = 0;
    wheelSpeedIntg_C      = 0;
}

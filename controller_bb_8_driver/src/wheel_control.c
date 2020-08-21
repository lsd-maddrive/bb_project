#include "tests.h"
#include "wheel_control.h"


static virtual_timer_t  wheel_control_vt;

// TODO: add permeation flag!!! in vt_cb
bool permition_flag = false;

// Array for control values in percent for each of wheels
lldControlValue_t       wheelControlValuesPrc[3] = {0, 0, 0};

wheelSpeedValue_t       wheelSpeedRefValuesPRS[3] = {0, 0, 0};

/**************** WHEEL A ********************/
//lldControlValue_t       wheelControlPrc_A     = 0;
wheelSpeedValue_t       wheelSpeedRefRPS_A    = 0;
// I-part with saturation
float                   wheelSpeedIntgCntr_A  = 0;
float                   wheelSpeedPrevError_A = 0;
float                   wheelSpeedIntg_A      = 0;


// TODO: fix values!!!
pidControllerValue_t    wheelController_A = {
    .kp = 40,
    .ki = 0,
    .kd = 0,
    .intgSaturation = 0,
    .propDeadZone = 0
};

/**************** WHEEL B ********************/
//lldControlValue_t       wheelControlPrc_B     = 0;
wheelSpeedValue_t       wheelSpeedRefRPS_B    = 0;
float                   wheelSpeedIntgCntr_B  = 0;
float                   wheelSpeedPrevError_B = 0;
float                   wheelSpeedIntg_B      = 0;


// TODO: fix values!!!
pidControllerValue_t    wheelController_B = {
    .kp = 0,
    .ki = 0,
    .kd = 0,
    .intgSaturation = 0,
    .propDeadZone = 0
};

/**************** WHEEL C ********************/
//lldControlValue_t       wheelControlPrc_C     = 0;
wheelSpeedValue_t       wheelSpeedRefRPS_C    = 0;
float                   wheelSpeedIntgCntr_C  = 0;
float                   wheelSpeedPrevError_C = 0;
float                   wheelSpeedIntg_C      = 0;


// TODO: fix values!!!
pidControllerValue_t    wheelController_C = {
    .kp = 0,
    .ki = 0,
    .kd = 0,
    .intgSaturation = 0,
    .propDeadZone = 0
};


static void wheel_control_vt_cb( void *arg )
{
    // to avoid warnings
    arg = arg;

/*============================================================================*/
/* Calculation Area                                                            */
/*============================================================================*/

/**************** CS WHEEL A ********************/
    odometrySpeedValue_t wheelSpeedRPS_A_LPF = odometryGetWheelSpeed( A, REVS_PER_SEC );
// P-Part
    float wheelSpeedError_A = wheelSpeedRefValuesPRS[0] - wheelSpeedRPS_A_LPF;
// How does it reflect on D-part? Maybe should be after D-calculation?
    if( wheelSpeedError_A <= wheelController_A.propDeadZone )
    {
        wheelSpeedError_A = 0;
        wheelSpeedIntgCntr_A = 0;
    }
// I-Part
    wheelSpeedIntg_A += wheelController_A.ki * wheelSpeedError_A;
    // I-part with saturation
    wheelSpeedIntgCntr_A = CLIP_VALUE(
          wheelSpeedIntg_A,
          -wheelController_A.intgSaturation,
          wheelController_A.intgSaturation
    );
// D-Part
    float wheelSpeedDif_A = wheelSpeedError_A - wheelSpeedPrevError_A;

    wheelSpeedPrevError_A = wheelSpeedError_A;

    wheelControlValuesPrc[0] = wheelController_A.kp * wheelSpeedError_A +
                               wheelController_A.ki * wheelSpeedIntgCntr_A +
                               wheelController_A.kd * wheelSpeedDif_A;

/**************** CS WHEEL B ********************/
    odometrySpeedValue_t wheelSpeedRPS_B_LPF = odometryGetWheelSpeed( B, REVS_PER_SEC );
// P-Part
    float wheelSpeedError_B = wheelSpeedRefValuesPRS[1] - wheelSpeedRPS_B_LPF;
// How does it reflect on D-part? Maybe should be after D-calculation?
    if( wheelSpeedError_B <= wheelController_B.propDeadZone )
    {
        wheelSpeedError_B = 0;
        wheelSpeedIntgCntr_B = 0;
    }
// I-Part
    wheelSpeedIntg_B += wheelController_B.ki * wheelSpeedError_B;
    float wheelSpeedIntgCntr_B = CLIP_VALUE(
          wheelSpeedIntg_B,
          -wheelController_B.intgSaturation,
          wheelController_B.intgSaturation
    );
// D-Part
    float wheelSpeedDif_B = wheelSpeedError_B - wheelSpeedPrevError_B;

    wheelSpeedPrevError_B = wheelSpeedError_B;

    wheelControlValuesPrc[1] = wheelController_B.kp * wheelSpeedError_B +
                               wheelController_B.ki * wheelSpeedIntgCntr_B +
                               wheelController_B.kd * wheelSpeedDif_B;

/**************** CS WHEEL C ********************/
    odometrySpeedValue_t wheelSpeedRPS_C_LPF = odometryGetWheelSpeed( C, REVS_PER_SEC );
// P-Part
    float wheelSpeedError_C = wheelSpeedRefValuesPRS[2] - wheelSpeedRPS_C_LPF;
// How does it reflect on D-part? Maybe should be after D-calculation?
    if( wheelSpeedError_C <= wheelController_C.propDeadZone )
    {
        wheelSpeedError_C = 0;
        wheelSpeedIntgCntr_C = 0;
    }
// I-Part
    wheelSpeedIntg_C += wheelController_C.ki * wheelSpeedError_C;
    wheelSpeedIntgCntr_C = CLIP_VALUE(
          wheelSpeedIntg_C,
          -wheelController_C.intgSaturation,
          wheelController_C.intgSaturation
    );
// D-Part
    float wheelSpeedDif_C = wheelSpeedError_C - wheelSpeedPrevError_C;

    wheelSpeedPrevError_C = wheelSpeedError_C;

    wheelControlValuesPrc[2] = wheelController_C.kp * wheelSpeedError_C +
                               wheelController_C.ki * wheelSpeedIntgCntr_C +
                               wheelController_C.kd * wheelSpeedDif_C;

/*============================================================================*/
/* Set control in percent                                                             */
/*============================================================================*/
    lldControlSetMotorPower( A, wheelControlValuesPrc[0] );
    lldControlSetMotorPower( B, wheelControlValuesPrc[1] );
    lldControlSetMotorPower( C, wheelControlValuesPrc[2] );


    chSysLockFromISR();
    chVTSetI(&wheel_control_vt, MS2ST( VT_WHEEL_CONTROL_MS ), wheel_control_vt_cb, NULL);
    chSysUnlockFromISR();
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

    chVTObjectInit(&wheel_control_vt);
    chVTSet( &wheel_control_vt, MS2ST( VT_WHEEL_CONTROL_MS ), wheel_control_vt_cb, NULL );

    isInitialized = true;
}

// TODO: CHECK wheelSpeedRefValuesPRS[number] !
/**
 * @brief       Set reference value of for specified wheel
 */
void wheelControlSetSpeed( wheelSpeedValue_t speed_val, motorNumberValue_t number, odometrySpeedUnit_t unit )
{
    speed_val = CLIP_VALUE( speed_val, WHEEL_SPEED_MAX_RPS, WHEEL_SPEED_MIN_RPS);

    switch( unit )
    {
        case REVS_PER_SEC:
            wheelSpeedRefValuesPRS[number] = speed_val;
            break;

        default:
            wheelSpeedRefValuesPRS[number] = 0; // temporary maybe
            break;
    }

//    switch( number )
//    {
//        case A:
//          switch( unit )
//          {
//              case REVS_PER_SEC:
//                  wheelSpeedRefRPS_A = speed_val;
//                  break;
//
//              default:
//                  break;
//          }
//          break;
//
//        case B:
//          switch( unit )
//          {
//              case REVS_PER_SEC:
//                  wheelSpeedRefRPS_B = speed_val;
//                  break;
//
//              default:
//                  break;
//          }
//          break;
//
//        case C:
//          switch( unit )
//          {
//              case REVS_PER_SEC:
//                  wheelSpeedRefRPS_C = speed_val;
//                  break;
//
//              default:
//                  break;
//          }
//          break;
//
//        default:
//          break;
//    }
}

// TODO: CHECK wheelControlValuesPrc[number] !
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
//    switch( number )
//    {
//        case A:
//          switch( unit )
//          {
//              case REVS_PER_SEC:
//                  return wheelControlPrc_A;
//                  break;
//
//              default:
//                  break;
//          }
//          break;
//
//        case B:
//          switch( unit )
//          {
//              case REVS_PER_SEC:
//
//                return wheelControlPrc_B;
//                  break;
//
//              default:
//                  break;
//          }
//          break;
//
//        case C:
//          switch( unit )
//          {
//              case REVS_PER_SEC:
//                  wheelSpeedRefRPS_C = speed_val;
//                  break;
//
//              default:
//                  break;
//          }
//          break;
//
//        default:
//          break;
//    }
}

/*
 * @brief       Reset all components for PID-controller
 */
void wheelControlResetController( motorNumberValue_t number )
{
    wheelSpeedRefValuesPRS[number] = 0;
    wheelControlValuesPrc[number] = 0;

    lldControlSetMotorPower( number, wheelControlValuesPrc[number] );

    switch( number )
    {
        case A:
          wheelSpeedIntgCntr_A  = 0;
          wheelSpeedPrevError_A = 0;
          wheelSpeedIntg_A      = 0;
          break;

        case B:
          wheelSpeedIntgCntr_B  = 0;
          wheelSpeedPrevError_B = 0;
          wheelSpeedIntg_B      = 0;
          break;

        case C:
          wheelSpeedIntgCntr_C  = 0;
          wheelSpeedPrevError_C = 0;
          wheelSpeedIntg_C      = 0;
          break;

        default:
          break;
    }
}

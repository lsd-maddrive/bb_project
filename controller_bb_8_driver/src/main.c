#include "common.h"
#include "tests.h"
#include <chprintf.h>

static inline void testsRoutines( void )
{
#if (MAIN_PROGRAM_ROUTINE == PROGRAM_MOTOR_DIRECTION_TEST)

    testRawMotorDirectionControlRoutine( );

#elif (MAIN_PROGRAM_ROUTINE == PROGRAM_MOTOR_RAW_CONTROL_TEST)

    testRawMotorControlRoutine( );

#elif (MAIN_PROGRAM_ROUTINE == PROGRAM_MOTOR_PRC_CONTROL_TEST)

    testMotorControlRoutine( );

#elif (MAIN_PROGRAM_ROUTINE == PROGRAM_MOTOR_FOR_GRISHA)

    testSpamPWM( );

#elif (MAIN_PROGRAM_ROUTINE == PROGRAM_ENCODER_TEST )

    testEncoderRoutine( ); 

#elif (MAIN_PROGRAM_ROUTINE == PROGRAM_MOTOR_MATLAB_FREQ_TEST )

    testMatlabFrequency();

#elif (MAIN_PROGRAM_ROUTINE == PROGRAM_WHEEL_SPEED_TEST )

    testWheelSpeed();

#endif
}

int main(void)
{
    chSysInit();
    halInit();

    #if (MAIN_PROGRAM_ROUTINE != PROGRAM_ROUTINE_MASTER)

        testsRoutines();

    #else
        // main routine
    #endif
}

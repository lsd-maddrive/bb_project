#include "tests.h"


static inline void testsRoutines( void )
{
#if (MAIN_PROGRAM_ROUTINE == PROGRAM_MOTOR_DIRECTION_TEST)

    testRawMotorDirectionControlRoutine( );

#elif (MAIN_PROGRAM_ROUTINE == PROGRAM_MOTOR_RAW_CONTROL_TEST)

    testRawMotorControlRoutine( );

#elif (MAIN_PROGRAM_ROUTINE == PROGRAM_MOTOR_PRC_CONTROL_TEST)

    testMotorControlRoutine( );

#elif (MAIN_PROGRAM_ROUTINE == PROGRAM_ENCODER_TEST )

    testEncoderRoutine( );

#elif (MAIN_PROGRAM_ROUTINE == PROGRAM_MOTOR_MATLAB_FREQ_TEST )

    testMatlabFrequency();

#elif (MAIN_PROGRAM_ROUTINE == PROGRAM_MOTOR_SPEED_TEST )

    testMotorSpeed();

#elif (MAIN_PROGRAM_ROUTINE == PROGRAM_WHEEL_SPEED_TEST )

    testWheelSpeed();

#elif (MAIN_PROGRAM_ROUTINE == PROGRAM_LINEAR_SPEED_MATLAB_TEST )

    testLinearSpeedMatlab();

#elif (MAIN_PROGRAM_ROUTINE == PROGRAM_ROBOT_ODOMETRY_TEST )

    testRobotOdometry();

#elif (MAIN_PROGRAM_ROUTINE == PROGRAM_WHEEL_CONTROL_MATLAB )

    testWheelControlMatlab();

#elif (MAIN_PROGRAM_ROUTINE == PROGRAM_ONE_WHEEL_CONTROLLER_MATLAB)

    testOneWheelControllerMatlab();

#elif (MAIN_PROGRAM_ROUTINE == PROGRAM_JOYSTICK_TEST )

    testJoystick();

#elif ( MAIN_PROGRAM_ROUTINE == PROGRAM_ROBOT_WITH_JOYSTICK_TEST )

    testRobotWithJoystick();

#elif (MAIN_PROGRAM_ROUTINE == PROGRAM_GYROSCOPE_TEST)

    testGyroscope();

#elif (MAIN_PROGRAM_ROUTINE == PROGRAM_LOGGER_TEST)

    testLogger();

#endif
}

int main(void)
{
    chSysInit();
    halInit();

    #if (MAIN_PROGRAM_ROUTINE != PROGRAM_ROUTINE_MASTER)

        testsRoutines();

    #else
        mainUnitsInit();
    #endif
}

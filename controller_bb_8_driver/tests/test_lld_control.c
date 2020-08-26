#include <tests.h>
#include <lld_control.h>

/*
 * @brief   Test motor direction control 
 * @note    Duty cycle is constant 
 */
void testRawMotorDirectionControlRoutine( void )
{
    debug_stream_init( );
    lldControlInit();

    lldMotorDirection_t test_dir    = FORWARD; 
    uint32_t            test_duty   = 5000;  

    systime_t   time = chVTGetSystemTimeX( );
    while( true )
    {
        char rcv_data   = sdGetTimeout( &SD3, TIME_IMMEDIATE );
        switch( rcv_data )
        {
            case 'a':
                test_dir = FORWARD;
                break;

            case 's':
                test_dir = BACKWARD;
                break;

            default:
                break;
        }
        lldControlSetRawMotorPower(A, test_duty, test_dir );
        lldControlSetRawMotorPower(B, test_duty, test_dir );
        lldControlSetRawMotorPower(C, test_duty, test_dir );
        dbgprintf( "DIR: (%d)\n\r", test_dir );

        time = chThdSleepUntilWindowed( time, time + MS2ST( 500 ) );
    }
}

 #define MOTOR_FORWARD
//#define MOTOR_BACKWARD

/*
 * @brief   Test raw motor control 
 * @note    Duty cycle could be changed [0, 20000]
 *          Stable for incorrect value of duty cycle 
 *          Direction is controlled via defines
 *          MOTOR_FORWARD || MOTOR_BACKWARD 
 */
void testRawMotorControlRoutine( void )
{
    debug_stream_init( ); 
    lldControlInit( );

    uint32_t test_duty1  = 0;
    uint32_t test_duty2  = 0;
    uint32_t test_duty3  = 0;

    uint32_t test_duty_delta = 500;

    systime_t   time = chVTGetSystemTimeX( );
    while( true )
    {
        char rcv_data   = sdGetTimeout( &SD3, TIME_IMMEDIATE ); 
        switch( rcv_data )
        {
            case 'q':
                test_duty1 += test_duty_delta;
                break;

            case 'w':
                test_duty1 -= test_duty_delta;
                break;

            case 'a':
                test_duty2 += test_duty_delta;
                break; 

            case 's':
                test_duty2 -= test_duty_delta;
                break;

            case 'z':
                test_duty3 += test_duty_delta;
                break; 

            case 'x':
                test_duty3 -= test_duty_delta;
                break;

            case ' ':
                test_duty1 = 0;
                test_duty2 = 0;
                test_duty3 = 0;
                break;
            
            default:
                break;
        }
        test_duty1 = CLIP_VALUE( test_duty1, 0, PWM_PERIOD );
        test_duty2 = CLIP_VALUE( test_duty2, 0, PWM_PERIOD );
        test_duty3 = CLIP_VALUE( test_duty3, 0, PWM_PERIOD );

#ifdef MOTOR_FORWARD
        lldControlSetRawMotorPower( A, test_duty1, FORWARD );
        lldControlSetRawMotorPower( B, test_duty2, FORWARD );
        lldControlSetRawMotorPower( C, test_duty3, FORWARD );

        dbgprintf("F D1:%d\tD2:%d\tD3:%d\n\r",
                  test_duty1, test_duty2, test_duty3);
#endif 

#ifdef MOTOR_BACKWARD
        lldControlSetRawMotorPower( 1, test_duty1, BACKWARD );
        lldControlSetRawMotorPower( 2, test_duty2, BACKWARD );
        lldControlSetRawMotorPower( 3, test_duty3, BACKWARD );
        dbgprintf("B D1:%d\tD2:%d\tD3:%d\n\r",
                  test_duty1, test_duty2, test_duty3);
#endif 
 
        time = chThdSleepUntilWindowed( time, time + MS2ST( 300 ) );
    }
}

/*
 * @brief   Test motor control in percents  
 * @note    Duty cycle could be changed [-100, 100]
 *          Stable for incorrect value of duty cycle 
 *          >= 0        FORWARD     
 *           < 0        BACKWARD
 */
void testMotorControlRoutine( void )
{
    debug_stream_init( ); 
    lldControlInit( );

    lldControlValue_t   test_duty1_prc   = 0; 
    lldControlValue_t   test_duty2_prc   = 0; 
    lldControlValue_t   test_duty3_prc   = 0; 

    lldControlValue_t   test_delta_prc   = 1;

    systime_t   time = chVTGetSystemTimeX( );
    while( true )
    {
        char rcv_data   = sdGetTimeout( &SD3, TIME_IMMEDIATE ); 

        switch( rcv_data )
        {
            case 'q':   // Motor 1
                test_duty1_prc += test_delta_prc;
                break; 
            
            case 'w':   // Motor 1
                test_duty1_prc -= test_delta_prc;
                break;

            case 'a':   // Motor 2
                test_duty2_prc += test_delta_prc;
                break; 
            
            case 's':   // Motor 2
                test_duty2_prc -= test_delta_prc;
                break;
            
            case 'z':   // Motor 3
                test_duty3_prc  += test_delta_prc;
                break; 
            
            case 'x':   // Motor 3
                test_duty3_prc  -= test_delta_prc;
                break; 

            case ' ':
                test_duty1_prc  = 0;
                test_duty2_prc  = 0;
                test_duty3_prc  = 0; 
                break; 
            
            default:
                break; 
        }
        test_duty1_prc = CLIP_VALUE( test_duty1_prc, LLD_MOTOR_MIN_PRC, LLD_MOTOR_MAX_PRC ); 
        lldControlSetMotorPower( A, test_duty1_prc );
        
        test_duty2_prc = CLIP_VALUE( test_duty2_prc, LLD_MOTOR_MIN_PRC, LLD_MOTOR_MAX_PRC ); 
        lldControlSetMotorPower( B, test_duty2_prc );

        test_duty3_prc = CLIP_VALUE( test_duty3_prc, LLD_MOTOR_MIN_PRC, LLD_MOTOR_MAX_PRC ); 
        lldControlSetMotorPower( C, test_duty3_prc );
        
        dbgprintf( "M1: (%d)\tM2: (%d)\tM3: (%d)\n\r", 
            test_duty1_prc, test_duty2_prc, test_duty3_prc );

        time = chThdSleepUntilWindowed( time, time + MS2ST( 300 ) );
    }
}

void testSpamPWM( void )
{
    lldControlInit( );

    systime_t   time = chVTGetSystemTimeX( );
    while( true )
    {
      lldControlSetMotorPower( A, 50 );
      time = chThdSleepUntilWindowed( time, time + MS2ST( 10 ) );
    }
}

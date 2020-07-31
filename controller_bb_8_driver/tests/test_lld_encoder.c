#include <tests.h>
#include <lld_encoder.h>
#include <lld_control.h>

// #define WITH_MOTOR
/**
 * @brief   Test common encoder routine 
 * @note    Print number of ticks, (int)revs and state of direction
 *          0   - clockwise 
 *          1   - counterclockwise 
 *          
 *          #define WITH_MOTOR enable you to use motor 
 */
void testEncoderRoutine( void )
{
    debug_stream_init( );
    lldEncoderInit( );

#ifdef WITH_MOTOR
    lldControlInit( );
    lldControlValue_t   test_duty_prc   = 0; 
    lldControlValue_t   test_delta_prc  = 10; 
#endif 
    // Motor 1
    encoderTicksValue_t test_ticks1  = 0;
    encoderRevsValue_t  test_revs1   = 0;
    bool                test_dir1    = 0;

    // Motor 2
    encoderTicksValue_t test_ticks2  = 0;
    encoderRevsValue_t  test_revs2   = 0;
    bool                test_dir2    = 0;

    // Motor 3
    encoderTicksValue_t test_ticks3  = 0;
    encoderRevsValue_t  test_revs3   = 0;
    bool                test_dir3    = 0;

    systime_t   time = chVTGetSystemTimeX( );
    while( true )
    {
        test_ticks1  = lldGetEncoderTicks( 1 );
        test_revs1   = lldGetEncoderRevs( 1 );
        test_dir1    = lldGetEncoderDirection( 1 );

        test_ticks2  = lldGetEncoderTicks( 2 );
        test_revs2   = lldGetEncoderRevs( 2 );
        test_dir2    = lldGetEncoderDirection( 2 );

        test_ticks3  = lldGetEncoderTicks( 3 );
        test_revs3   = lldGetEncoderRevs( 3 );
        test_dir3    = lldGetEncoderDirection( 3 );

#ifdef WITH_MOTOR
        char rcv_data   = sdGetTimeout( &SD3, TIME_IMMEDIATE ); 

        switch( rcv_data )
        {
            case 'a':
                test_duty_prc += test_delta_prc;
                break; 
            
            case 's':
                test_duty_prc -= test_delta_prc;
                break;

            case ' ':
                test_duty_prc = 0;
                break; 
            
            default:
                break; 
        }
        test_duty_prc = CLIP_VALUE( test_duty_prc, LLD_MOTOR_MIN_PRC, LLD_MOTOR_MAX_PRC ); 
        lldControlSetMotorPower( 1, test_duty_prc );

        dbgprintf( "Power: (%d)\tTicks: (%d)\tRevs: (%d)\tDir: (%d)\n\r",
                    test_duty_prc, test_ticks, (int32_t)test_revs, test_dir ); 
#endif

#ifndef WITH_MOTOR
        dbgprintf( "T1: %d\tT2: %d\tT3: %d\n\r"
                   "R1: %d\tR2: %d\tR3: %d\n\r"
                   "D1: %d\tD2: %d\tD3: %d\n\r",
                   test_ticks1, test_ticks2, test_ticks3,
                   (int32_t)test_revs1, (int32_t)test_revs2, (int32_t)test_revs3,
                   test_dir1, test_dir2, test_dir3);
#endif 

        time = chThdSleepUntilWindowed( time, time + MS2ST( 600 ) );
    }
}


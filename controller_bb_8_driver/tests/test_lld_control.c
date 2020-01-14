#include <tests.h>
#include <lld_control.h>


void testRawMotorControlRoutine( void )
{
    debug_stream_init( ); 
    lldControlInit( );

    systime_t   time = chVTGetSystemTimeX( );
    while( true )
    {
        char rcv_data   = sdGetTimeout( &SD3, TIME_IMMEDIATE ); 
        char test = ''; 
        switch( rcv_data )
        {
            case 'a':
                test = 'a';
                break; 

            case 's':
                test = 's'; 
                break; 
        }

        dbgprintf("(%c)\n\r", test); 
        time = chThdSleepUntilWindowed( time, time + MS2ST( 500 ) );
    }
}
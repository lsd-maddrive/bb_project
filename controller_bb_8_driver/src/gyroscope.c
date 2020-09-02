#include <gyroscope.h>


static bool             isInitialized       = false;

static bool             timerFlag           = false;
static virtual_timer_t  gyroscope_vt;
float                   gyro_angle_xyz[3]   = {0, 0, 0};
float                   angular_speed[3]    = {0, 0, 0};
float                   gyro_mean_error[3]  = {0, 0, 0};

/**
 * @brief   Initialize gyroscope
 */
void gyroscopeInit(void)
{
    if(isInitialized)
        return;

    uint8_t initbuf[5];
    initbuf[0] = GYRO_CTRL1_REG;            // Start from ctrl1 register
    initbuf[1] = 0x0F;                      // Normal mode, each axis enable, freq settings ignored
    initbuf[2] = 0x00;                      // High pass filter default configuration
    initbuf[3] = 0x00;                      // Too smart for me
    initbuf[4] = 0x30;                      // 2000dps

    chVTObjectInit( &gyroscope_vt );
    chVTSet( &gyroscope_vt, MS2ST( GYRO_INT_PERIOD ), gyroIntegrationCallback, NULL );
    
    i2cSimpleWrite(GYRO_ADDR, initbuf, 5, 1000);

    calculateGyroError(gyro_mean_error);

    isInitialized = true;
}


/**
 * @brief   Callback function that calculates angle
 */
static void gyroIntegrationCallback(void *args)
{
    args = args;

    uint8_t i;
    msg_t msg = readGyroSpeed(angular_speed);

    if( msg == MSG_OK )
    {
        for(i = 0; i < 3; i++)
        {
            angular_speed[i] -= gyro_mean_error[i];

            if (abs(angular_speed[i]) < 0.01)
                angular_speed[i] = 0;

            gyro_angle_xyz[i] += angular_speed[i] * GYRO_INT_PERIOD;
        }
    }
    

    chSysLockFromISR();
    chVTSetI(&gyroscope_vt, MS2ST(GYRO_INT_PERIOD), gyroIntegrationCallback, NULL);
    chSysUnlockFromISR();
}


/**
 * @brief   Get current angle value, [deg]
 *
 * @param
 *          axis        Number of axis to return. 0 through 2 for XYZ
 */
float getGyroAngle(uint8_t axis)
{
    if(axis < 3)
        return gyro_angle_xyz[axis];
    return -1;
}


/**
 * @brief   Start calculation of angle
 */
void startGyroPosition(void)
{
    if(timerFlag)
        return;

    gyroscopeInit();
    chVTObjectInit(&gyroscope_vt);
    chVTSet( &gyroscope_vt, MS2ST(GYRO_INT_PERIOD), gyroIntegrationCallback, NULL );

    timerFlag = true;
}


/**
 * @brief   Stop calculation of angle
 */
void stopGyroPosition(void)
{
    timerFlag = false;
    gyro_angle_xyz[0] = 0;
    gyro_angle_xyz[1] = 0;
    gyro_angle_xyz[2] = 0;
}

/**
 * @brief   Read gyroscope axis values in XYZ order
 *
 * @param
 *          *axis_values         Array where to store read data
 */
msg_t readGyroscope(int16_t *axis_values)
{
    uint8_t gyro_temp[6] = {0, 0, 0, 0, 0, 0};
    msg_t msg = i2cRegisterRead(GYRO_ADDR, GYRO_DATA_REG, gyro_temp, 6, 10000);

    
    uint8_t i = 0;
    for(i = 0; i < 3; i++)
    {
        axis_values[i] = (int16_t)((uint16_t)(gyro_temp[i * 2]) | ((uint16_t)(gyro_temp[(i * 2) + 1]) << 8));
    }
    return msg;
}

/**
 * @brief   Read gyroscope and represent values as int16
 *
 * @param
 *          *axis_speed         Array where to store axial rotational speed values
 */
msg_t readGyroSpeed(float *axis_speed)
{
    int16_t gyro_axis_values[3] = {0, 0, 0};
    uint8_t i = 0;
    msg_t msg = readGyroscope(gyro_axis_values);
    for(i = 0; i < 3; i++)
    {
        axis_speed[i] = (float)gyro_axis_values[i] * 0.07;
    }
    return msg;
}


/**
 * @brief   Calculate static error
 * @details Calculates average value of each axis data over ten readings
 *
 * @param
 *          *axis_speed         Array where to store static error values
 */
msg_t calculateGyroError(float *buf)
{
    msg_t msg = MSG_OK;
    int16_t temp_buf[10][3] = {{0}};
    int16_t super_temp[3] = {0, 0, 0};
    uint8_t i, j;
    float x_mean = 0, y_mean = 0, z_mean = 0;
    for(i = 0; i < 10; i++)
    {
        msg = readGyroscope(super_temp);
        for(j = 0; j < 3; j++)
        {
            temp_buf[i][j] = super_temp[j];
        }
    }


    for(i = 0; i < 10; i++)
    {
        x_mean += temp_buf[i][0];
        y_mean += temp_buf[i][1];
        z_mean += temp_buf[i][2];
    }

    buf[0] = (x_mean / 10) * 0.07;
    buf[1] = (y_mean / 10) * 0.07;
    buf[2] = (z_mean / 10) * 0.07;

    return msg;
}

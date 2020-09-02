#ifndef GYROSCOPE_H_
#define GYROSCOPE_H_

#include <common.h>

#define GYRO_ADDR       0x6B    // i2c address of gyroscope
#define GYRO_CTRL1_REG  0xA0    // address of control register with autoincrement bit set
#define GYRO_DATA_REG   0xA8    // address of OUT_X_L register with autoincrement bit set
#define GYRO_INT_PERIOD 400


/**
 * @brief   Read gyroscope axis values in XYZ order
 *
 * @param
 *          *axis_values         Array where to store read data
 */
msg_t readGyroscope(int16_t *axis_values);


/**
 * @brief   Read gyroscope and represent values as int16
 *
 * @param
 *          *axis_speed         Array where to store axial rotational speed values
 */
msg_t readGyroSpeed(float *axis_speed);


/**
 * @brief   Calculate static error
 * @details Calculates average value of each axis data over ten readings
 *
 * @param
 *          *axis_speed         Array where to store static error values
 */
msg_t calculateGyroError(float *buf);


/**
 * @brief   Stop calculation of angle
 */
void stopGyroPosition(void);


/**
 * @brief   Start calculation of angle
 */
void startGyroPosition(void);


/**
 * @brief   Get current angle value, [deg]
 *
 * @param
 *          axis        Number of axis to return. 0 through 2 for XYZ
 */
float getGyroAngle(uint8_t axis);


/**
 * @brief   Callback function that calculates angle
 */
static void gyroIntegrationCallback(void *args);


/**
 * @brief   Initialize gyroscope
 */
void gyroscopeInit(void);


#endif /* GYROSCOPE_H_ */

#ifndef GYROSCOPE_H_
#define GYROSCOPE_H_

#include <common.h>

#define GYRO_ADDR       0x6B    // i2c address of gyroscope
#define GYRO_CTRL1_REG  0xA0    // address of control register with autoincrement bit set
#define GYRO_DATA_REG   0xA8    // address of OUT_X_L register with autoincrement bit set


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
 * @brief   Initialize gyroscope
 */
void gyroscopeInit(void);


#endif /* GYROSCOPE_H_ */

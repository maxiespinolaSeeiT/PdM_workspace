/*
 * BMP280_driver.h
 *
 *  Created on: Apr 11, 2026
 *      Author: Usuario
 */

#ifndef BMP280_DRIVERS_INC_BMP280_DRIVER_H_
#define BMP280_DRIVERS_INC_BMP280_DRIVER_H_

#include <stdint.h>
#include "BMP280_port.h"
#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

typedef bool bool_t;


bool_t BMP280_Init(void);
//bool_t BMP280_ReadData(int32_t *temperature, uint32_t *pressure);
bool_t BMP280_ReadData(float *temperature, float  *pressure);



#endif /* BMP280_DRIVERS_INC_BMP280_DRIVER_H_ */

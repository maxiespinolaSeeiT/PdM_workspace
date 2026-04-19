/*
 * BMP280_driver.h
 *
 *  Created on: Apr 11, 2026
 *      Author: Usuario
 */

#ifndef BMP280_DRIVERS_INC_BMP280_DRIVER_H_
#define BMP280_DRIVERS_INC_BMP280_DRIVER_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>
#include "../../../Devices/BMP280/Inc/BMP280_port.h"

typedef bool bool_t;

//bool_t BMP280_ReadData(float *temperature, float  *pressure);
//bool_t BMP280_Task(float *temperature, float *pressure);
void BMP280_Update();
bool_t BMP280_IsReady(void);
bool_t BMP280_GetData(float *t, float *p);
void BMP_Init();
void BMP_Start(void);



#endif /* BMP280_DRIVERS_INC_BMP280_DRIVER_H_ */

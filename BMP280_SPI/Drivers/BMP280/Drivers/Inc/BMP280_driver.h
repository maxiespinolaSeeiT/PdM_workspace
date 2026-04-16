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

typedef struct {
    uint16_t dig_T1;
    int16_t  dig_T2;
    int16_t  dig_T3;

    uint16_t dig_P1;
    int16_t  dig_P2;
    int16_t  dig_P3;
    int16_t  dig_P4;
    int16_t  dig_P5;
    int16_t  dig_P6;
    int16_t  dig_P7;
    int16_t  dig_P8;
    int16_t  dig_P9;
} BMP280_CalibData;

static BMP280_CalibData calib;
static int32_t t_fine;



void BMP280_Init(void);
float BMP280_ReadTemperature(void);
float BMP280_ReadPressure(void);



#endif /* BMP280_DRIVERS_INC_BMP280_DRIVER_H_ */

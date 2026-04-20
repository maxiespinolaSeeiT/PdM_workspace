/*
 * BMP280_port.h
 *
 *  Created on: Apr 11, 2026
 *      Author: Maximiliano Ariel Espínola
 */

#ifndef BMP280_DRIVERS_INC_BMP280_PORT_H_
#define BMP280_DRIVERS_INC_BMP280_PORT_H_
#include "stdint.h"

//------Estados del sensor-------//
typedef enum {
    BMP280_OK = 0,
    BMP280_ERROR
} bmp280_status_t;

//--------Public functions-----------------//
bmp280_status_t BMP280_ReadReg(uint8_t reg, uint8_t *data, uint16_t size);
bmp280_status_t BMP280_WriteReg(uint8_t reg, uint8_t *data, uint16_t size);

void BMP280_Delay(uint32_t ms);
#endif /* BMP280_DRIVERS_INC_BMP280_PORT_H_ */

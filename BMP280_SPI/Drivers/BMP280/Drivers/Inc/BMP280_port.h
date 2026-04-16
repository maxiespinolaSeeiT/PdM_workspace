/*
 * BMP280_port.h
 *
 *  Created on: Apr 11, 2026
 *      Author: Usuario
 */

#ifndef BMP280_DRIVERS_INC_BMP280_PORT_H_
#define BMP280_DRIVERS_INC_BMP280_PORT_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>

// Init HW
void BMP280_Port_Init(void);

// SPI
void BMP280_SPI_Write(uint8_t *data, uint16_t len);
void BMP280_SPI_Read(uint8_t *txData, uint8_t *rxData, uint16_t len);

// Chip Select
void BMP280_CS_Select(void);
void BMP280_CS_Deselect(void);

// UART (debug)
void BMP280_UART_Send(char *msg);


#endif /* BMP280_DRIVERS_INC_BMP280_PORT_H_ */

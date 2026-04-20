/*
 * I2C_port.h
 *
 *  Created on: Apr 15, 2026
 *      Author: Usuario
 */

#ifndef API_INC_API_I2C_H_
#define API_INC_API_I2C_H_

#include "stm32f4xx_hal.h"
#include "API_common_types.h"

void Error_Handler(void);

bool_t I2C_init(void);
void I2C_Force_Restart(void);
bool_t bsp_i2c_write(uint8_t *data, uint16_t size, uint16_t address);
bool_t bsp_i2c_read(uint8_t *data, uint16_t size, uint16_t address);
void bsp_i2c_delay(uint32_t);
#endif /* API_INC_API_I2C_H_ */

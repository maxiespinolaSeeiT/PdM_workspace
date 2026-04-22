/*
 * I2C_port.h
 *
 *  Created on: Apr 15, 2026
 *      Author: Maximiliano Ariel Espinola
 */

#ifndef API_INC_API_I2C_H_
#define API_INC_API_I2C_H_

#include "API_common_types.h"
#include "stm32f4xx_hal.h"


//--------Public functions-----------------//
void Error_Handler(void);
bool_t I2C_init(void);
void I2C_Force_Restart(void);
bool_t bsp_i2c_write(uint8_t *data, uint16_t size, uint16_t address); //Envia un comando por I2C
bool_t bsp_i2c_read(uint8_t *data, uint16_t size, uint16_t address); //Lee todo el buffer de I2C
bool_t bsp_i2c_mem_read(uint8_t reg, uint8_t *data, uint16_t size, uint16_t address); //Funcion para leer registros por I2C
void bsp_i2c_delay(uint32_t);//Delay bloqueante
uint32_t bsp_i2c_getTick(void);//Toma el tiempo del ck del sistema
I2C_HandleTypeDef bsp_i2c_getConfig(); //Getter de struct de configuracion del I2C
bool_t bsp_i2c_isDeviceReady(I2C_HandleTypeDef *I2C_config, uint16_t num_device); //Consultar estado del puerto

#endif /* API_INC_API_I2C_H_ */

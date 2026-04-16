/*
 * LCD_port.h
 *
 *  Created on: Apr 9, 2026
 *      Author: Usuario
 */

#ifndef API_INC_LCD_PORT_H_
#define API_INC_LCD_PORT_H_

#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "I2C_port.h"

#define ADDR_I2C_LCD_PCF8574T 0x27 //Configurar dirección I2C del módulo


// Inicializa el puerto (I2C)
void LCD_Port_Init(void);

// Envía un byte al expansor (PCF8574)
void LCD_Port_Write(uint8_t data);

// Delay en ms
void LCD_Port_Delay(uint32_t ms);

void MX_I2C1_Init(void);

void Error_Handler_LCDPort(void);

#endif /* API_INC_LCD_PORT_H_ */
